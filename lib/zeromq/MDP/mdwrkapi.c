#include "mdwrkapi.h"

#define HEARTBEAT_LIVENESS 3

struct _mdwrk_t {
	zctx_t *ctx;
	char *broker;
	char *service;
	void *worker;
	int verbose;

	uint64_t heartbeat_at;
	size_t liveness;
	int heartbeat;
	int reconnect;

	int expect_reply;
	zframe_t *reply_to;
};

static void
s_mdwrk_send_to_broker(mdwrk_t *self, char *command, char *option, zmsg_t *msg)
{
	msg = msg ? zmsg_dup(msg) : zmsg_new();

	if (option)
		zmsg_pushstr(msg, option);
	zmsg_pushstr(msg, command);
	zmsg_pushstr(msg, MDPW_WORKER);
	zmsg_pushstr(msg, "");

	if (self->verbose){
		zclock_log("I: sending %s to broker", mdps_commands[(int)*command]);
		zmsg_dump(msg);
	}
	zmsg_send(&msg, self->worker);
}

void 
s_mdwrk_connect_to_broker(mdwrk_t *self)
{
	if (self->worker)
		zsocket_destroy(self->ctx, self->worker);
	self->worker = zsocket_new(self->ctx, ZMQ_DEALER);
	zmq_connect(self->worker, self->broker);
	if (self->verbose)
		zclock_log("I: connecting to broker at %s...", self->broker);

	s_mdwrk_send_to_broker(self, MDPW_READY, self->service, NULL);

	self->liveness = HEARTBEAT_LIVENESS;
	self->heartbeat_at = zclock_time() + self->heartbeat;
}

mdwrk_t *
mdwrk_new (char *broker, char *service, int verbose)
{
	assert(broker);
	assert(service);

	mdwrk_t *self = (mdwrk_t *)zmalloc(sizeof(mdwrk_t));
	self->ctx = zctx_new();
	self->broker = strdup(broker);
	self->service = strdup(service);
	self->verbose = verbose;
	self->heartbeat = 2500;
	self->reconnect = 2500;

	s_mdwrk_connect_to_broker(self);
	return self;
}

void 
mdwrk_destroy(mdwrk_t **self_p)
{
	assert(self_p);
	if (*self_p){
		mdwrk_t *self = *self_p;
		zctx_destroy(&self->ctx);
		free(self->broker);
		free(self->service);
		free(self);
		*self_p = NULL;
	}
}

void
mdwrk_set_heartbeat(mdwrk_t *self, int heartbeat)
{
	self->heartbeat = heartbeat;
}

void 
mdwrk_set_reconnect(mdwrk_t *self, int reconnect)
{
	self->reconnect = reconnect;
}

zmsg_t *
mdwrk_recv(mdwrk_t *self, zmsg_t **reply_p)
{
	assert(reply_p);
	zmsg_t *reply = *reply_p;
	assert(reply || !self->expect_reply);
	if (reply){
		assert(self->reply_to);
		zmsg_wrap(reply, self->reply_to);
		s_mdwrk_send_to_broker(self, MDPW_REPLY, NULL, reply);
		zmsg_destroy(reply_p);
	}
	self->expect_reply = 1;

	while (true){
		zmq_pollitem_t items [] = {
			{self->worker, 0, ZMQ_POLLIN, 0}
		};
		int rc = zmq_poll(itmes, 1, self->hearbeat * ZMQ_POLL_MSEC);
		if (rc == -1)
			break;

		if (items[0].revents & ZMQ_POLLIN){
			zmsg_t *msg = zmsg_recv(self->worker);
			if (!msg)
				break;
			if (self->verbose){
				zclock_log("I: received messge from broker:");
				zsg_dump(msg);
			}
			self->liveness = HEARBEAT_LIVENESS;

			assert(zmsg_size(msg) >= 3);

			zframe_t *empty = zmsg_pop(msg);
			assert(zframe_streq(empty, ""));
			zframe_destroy(&empty);

			zframe_t *header = zmsg_pop(msg);
			assert(zframe_streq(header, MDPW_WORKER));
			zframe_destroy(&header);

			zframe_t *command = zmsg_pop(msg);
			if (zframe_streq(command, MDPW_REQUEST)){
				self->reply_to = zmsg_unwrap(msg);
				zframe_destroy(&command);
				return msg;
			}
			else 
			if (zframe_streq(command, MDPW_HEARTBEAT))
				;
			else
			if (zframe_streq(command, MDPW_DISCONNECT))
				s_mdwrk_connect_to_broker(self);
			else {
				zclock_log("E: invalid input message");
				zmsg_dump(msg);
			}
			zframe_destroy(&command);
			zmsg_destroy(&msg);
		}
		else 
		if (--self->liveness == 0){
			if (self->verbose)
				zclock_log("W: disconnected from broker - retrying...");
			zclock_sleep(self->reconnect);
			s_mdwrk_connect_to_broker(self);
		}

		if (zclock_time() > self->hearbeat_at){
			s_mdwrk_send_to_broker(self, MDPW_HEARTBEAT, NULL, NULL);
			self->heartbeat_at = zclock_time() + self->heartbeat;
		}
	}
	if (zctx_interrupted)
		printf("W: interrupt received , killing worker...\n");
	return NULL;
}









