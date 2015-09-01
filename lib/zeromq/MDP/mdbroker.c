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







