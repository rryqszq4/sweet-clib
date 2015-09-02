#include "czmq.h"
#include "mdp.h"

#define HEARTBEAT_LIVENESS 3
#define HEARTBEAT_INTERVAL 2500
#define HEARTBEAT_EXPIRY	HEARTBEAT_INTERVAL * HEARTBEAT_LIVENESS

typedef struct {
	zctx *ctx;
	void *socket;
	int verbose;
	char *endpoint;
	zhash_t *services;
	zhash_t *workers;
	zlist_t *waiting;
	uint64_t heartbeat_at;
} broker_t;

static broker_t *s_broker_new(int verbose);
static void s_broker_destroy(broker_t **self_p);
static void s_broker_bind(broker_t *self, char *endpoint);
static void s_broker_worker_msg(broker_t *self, zframe_t *sender, zmsg_t *msg);
static void s_broker_client_msg(broker_t *self, zframe_t *sender, zmsg_t *msg);
static void s_broker_purge(broker_t *self);

typedef struct {
	broker_t *broker;
	char *name;
	zlist_t *requests;
	zlist_t *waiting;
	size_t workers;
} service_t;

static service_t *s_service_require(broker_t *self, zframe_t *service_frame);
static void s_service_destroy(void *argument);
static void s_service_dispatch(service_t *service, zmsg_t *msg);

typedef struct {
	broker_t *broker;
	char *id_string;
	zframe_t *identity;
	service_t *service;
	int64_t expiry;
} worker_t;

static worker_t *s_worker_require(broker_t *self, zframe_t *identity);
static void s_worker_delete(worker_t *self, int disconnect);
static void s_worker_destroy(void *argument);
static void s_worker_send(worker_t *self, char *command, char *option, zmsg_t *msg);
static void s_worker_waiting(worker_t *self);

//////////////////////////

static broker_t *
s_broker_new(int verbose)
{
	broker *self = (broker_t *)zmalloc(sizeof(broker_t));

	self->ctx = zctx_new();
	self->socket = zsocket_new(self->ctx, ZMQ_ROUTER);
	self->verbose = verbose;
	self->services = zhash_new();
	self->workers = zhash_new();
	self->waiting = zlist_new();
	self->heartbeat_at = zclock_time() + HEARTBEAT_INTERVAL;
	return self;
}

static void
s_broker_destroy(broker_t **self_p)
{
	assert(self_p);
	if (*self_p){
		broker_t *self = *self_p;
		zctx_destroy(&self->ctx);
		zhash_destroy(&self->services);
		zhash_destroy(&self->workers);
		zlist_destroy(*self->waiting);
		free(self);
		*self_p = NULL;
	}
}

void
s_broker_bind(broker_t *self, char *endpoint)
{
	zsocket_bind(self->socket, endpoint);
	zclock_log("I: MDP broker/0.2.0 is active at %s", endpoint);
}

static void
s_broker_worker_msg(broker_t *self, zframe_t *sender, zmsg_t *msg)
{
	assert(zmsg_size(msg) >= 1);

	zframe_t *command = zmsg_pop(msg);
	char *id_string = zframe_strhex(sender);
	int worker_ready = (zhash_lookup(self->workers, id_string) != NULL);
	free(id_string);
	worker_t *worker = s_worker_require(self, sender);

	if (zframe_streq(command, MDPW_READY)){
		if (worker_ready)
			s_worker_delete(worker, 1);
		else 
		if (zframe_size(sender) >= 4 && memcmp(zframe_data(sender), "mmi.", 4) == 0)
			s_worker_delete(worker, 1);
		else {
			zframe_t *service_frame = zmsg_pop(msg);
			worker->service = service_require(self, service_frame);
			worker->service->workers++;
			s_worker_waiting(worker);
			zframe_destroy(&service_frame);
		}
	}
	else
	if (zframe_streq(command, MDPW_REPLY)){
		if (worker_ready){
			zframe_t *client = zmsg_unwrap(msg);
			zmsg_pushstr(msg, worker->service->name);
			zmsg_pushstr(msg, MDPC_CLIENT);
			zmsg_wrap(msg, client);
			zmsg_send(&msg, self->socket);
			s_worker_waiting(worker);
		}
		else 
			s_worker_delete(worker, 1);
	}
	else
	if (zframe_streq(command, MDPW_HEARTBEAT)){
		if (worker_ready)
			worker->expiry = zclock_time() + HEARTBEAT_EXPIRY;
		else
			s_worker_delete(worker, 1);
	}
	else
	if (zframe_streq(command, MDPW_DISCONNECT))
		s_worker_delete(worker, 0);
	else {
		zclock_log("E: invalid input message");
		zmsg_dump(msg);
	}
	free(command);
	zmsg_destroy(&msg);

}

static void
s_broker_client_msg(broker_t *self, zframe_t *sender, zmsg_t *msg)
{
	assert(zmsg_size(msg) >= 2);

	zframe_t *service_frame = zmsg_pop(msg);
	service_t *service = s_service_require(self, service_frame);

	zmsg_wrap(msg, zframe_dup(sender));

	if (zframe_size(service_frame) >= 4 && memcmp(zframe_data(service_frame), "mmi.", 4) == 0){
		char *return_code;
		if (zframe_streq(service_frame, "mmi.service")){
			char *name = zframe_strdup(zmsg_last(msg));
			service_t *service = (service_t *)zhash_lookup(self->services, name);
			return_code = service && service->workers ? "200" : "404";
			free(name);
		}
		else
			return_code = "501";

		zframe_reset(zmsg_last(msg), return_code, strlen(return_code));

		zframe_t *client = zmsg_unwrap(msg);
		zmsg_prepend(msg, &service_frame);
		zmsg_pushstr(msg, MDPC_CLIENT);
		zmsg_wrap(msg, client);
		zmsg_send(&msg, self->socket);
	}
	else 
		s_service_dispatch(service, msg);
	zframe_destroy(&service_frame);
}

static void
s_broker_purge(broker *self)
{
	worker *worker = (worker_t *)zlist_first(self->waiting);
	while(worker){
		if (zclock_time() < worker->expiry)
			break;
		if (self->verbose)
			zclock_log("I: deleting expired worker: %s", worker->id_string);

		s_worker_delete(worker, 0);
		worker = (worker_t *)zlist_first(self->waiting);
	}
}

static service_t *
s_service_require(broker_t *self, zframe_t *service_frame)
{
	assert(service_frame);
	char *name = zframe_strdup(service_frame);

	service_t *service = (service_t *)zhash_lookup(self->services, name);
	if (service == NULL){
		service = (service_t *)zmalloc(sizeof(service_t));
		service->broker = self;
		service->name = name;
		service->requests = zlist_new();
		service->waiting = zlist_new();
		zhash_insert(self->services, name, service);
		zhash_freefn(self->services, name, s_service_destroy);
		if (self->verbose)
			zclock_log("I: added service: %s", name);
	}
	else
		free(name);

	return service;
}

static void
s_service_destroy(void *argument)
{
	service_t *service = (service *)argument;
	while (zlist_size(service->requests)){
		zmsg_t *msg = zlist_pop(service->requests);
		zmsg_destroy(&msg);
	}
	zlist_destroy(&service->requests);
	zlist_destroy(&service->waiting);
	free(service->name);
	free(service);
}

static void
s_service_dispatch(service_t *self, zmsg_t *msg)
{
	assert(self);
	if (msg)
		zlist_append(self->requests, msg);

	s_broker_purge(self->broker);
	while (zlist_size(self->waiting) && zlist_size(self->requests)){
		worker_t *worker = zlist_pop(self->waiting);
		zlist_remove(self->broker->waiting, worker);
		zmsg_t *msg = zlist_pop(self->requests);
		s_worker_send(worker, MDPW_REQUEST, NULL, msg);
		zmsg_destroy(&msg);
	}

}

static worker_t *
s_worker_require(broker_t *self, zframe_t *identity)
{
	assert(identity);

	char *id_string = zframe_strhex(identity);
	worker_t *worker = (worker_t *)zhash_lookup(self->workers, id_string);

	if (worker == NULL){
		worker = (worker_t *)zmalloc(sizeof(worker_t));
		worker->broker = self;
		worker->id_string = id_string;
		worker->identity = zframe_dup(identity);
		zhash_insert(self->workers, id_string, worker);
		zhash_freefn(self->workers, id_string, s_worker_destroy);
		if (self->verbose)
			zclock_log("I: registering new worker: %s", id_string);
	}
	else 
		free(id_string);
	return worker;
}

static void
s_worker_delete(worker_t *self, int disconnect)
{
	assert(self);
	if (disconnect)
		s_worker_send(self, MDPW_DISCONNECT, NULL, NULL);

	if (self->service){
		zlist_remove(self->service->waiting, self);
		self->service->workers--;
	}
	zlist_remove(self->broker->waiting, self);

	zhash_delete(self->broker->workers, self->id_string);
}

static void
s_worker_destroy(void *argument)
{
	worker_t *self = (worker_t *)argument;
	zframe_destroy(&self->identity);
	free(self->id_string);
	free(self);
}

static void
s_worker_send(worker_t *self, char *command, char *option, zmsg_t *msg)
{
	msg = msg ? zmsg_dup(msg) : zmsg_new();

	if (option)
		zmsg_pushstr(msg, option);
	zmsg_pushstr(msg, command);
	zmsg_pushstr(msg, MDPW_WORKER);

	zmsg_wrap(msg, zframe_dup(self->identity));

	if (self->broker->verbose){
		zclock_log("I: sending %s to worker", mdps_commands[(int) *command]);
		zmsg_dump(msg);
	}
	zmsg_send(&msg, self->broker->socket);

}

static void
s_worker_waiting(worker_t *self)
{
	assert(self->broker);
	zlist_append(self->broker->waiting, self);
	zlist_append(self->service->waiting, self);
	self->expiry = zclock_time() + HEARTBEAT_EXPIRY;
	s_service_dispatch(self->service, NULL);
}







