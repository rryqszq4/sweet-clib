#include "mdcliapi.h"

struct _mdcli_t
{
	zctx_t *ctx;	//上下文
	char *broker;
	void *client;
	int verbose;	//打印标准输出
	int timeout;
	int retries;
};

void s_mdcli_connect_to_broker(mdcli_t *self)
{
	if (self->client)
		zsocket_destroy(self->ctx, self->client);
	self->client = zsocket_new(self->ctx, ZMQ_REQ);
	zmq_connect(self->client, self->broker);
	if (self->verbose)
		zclock_log("I: connecting to broker at %s...", self->broker);
}

mdcli_t *
mdcli_new(char *broker, int verbose)
{
	assert(broker);

	mdcli_t *self = (mdcli_t *)zmalloc(sizeof(mdcli_t));
	self->ctx = zctx_new();
	self->broker = strdup(broker);
	self->verbose = verbose;
	self->timeout = 2500;
	self->retries = 3;

	s_mdcli_connect_to_broker(self);
	return self;
}

void 
mdcli_destroy(mdcli_t **self_p)
{
	assert(self_p);
	if (*self_p){
		mdcli_t *self = *self_p;
		zctx_destroy(&self->ctx);
		free(self->broker);
		free(self);
		*self_p = NULL;
	}
}

void 
mdcli_set_timeout(mdcli_t *self, int timeout)
{
	assert(self);
	self->timeout = timeout;
}

void
mdcli_set_retries(mdcli_t *self, int retries)
{
	assert(self);
	self->retries = retries;
}

zmsg_t *
mdcli_send(mdcli_t *self, char *service, zmsg_t **request_p)
{
	assert(self);
	assert(request_p);
	zmsg_t *request = *request_p;

	zmsg_pushstr(request, service);
	zmsg_pushstr(request, MDPC_CLIENT);
	if (self->verbose){
		zclock_log("I: send request to '%s' service:", service);
		zmsg_dump(request);
	}
	int retries_left = self->retries;
	while (retries_left && !zctx_interrupted){
		zmsg_t *msg = zmsg_dup(request);
		zmsg_send(&msg, self->client);

		zmq_pollitem_t items [] = {
			{self->client, 0, ZMQ_POLLIN, 0}
		};

		int rc = zmq_poll(items, 1, self->timeout * ZMQ_POLL_MSEC);
		if (rc == -1)
			break;

		if (items[0].revents & ZMQ_POLLIN){
			zmsg_t *msg = zmsg_recv(self->client);
			if (self->verbose){
				zclock_log("I: received reply:");
				zmsg_dump(msg);
			}

			assert(zmsg_size(msg) >= 3);

			zframe_t *header = zmsg_pop(msg);
			assert(zframe_streq(header, MDPC_CLIENT));
			zframe_destroy(&header);

			zframe_t *reply_service = zmsg_pop(msg);
			assert(zframe_streq(reply_service, service));
			zframe_destroy(&reply_service);

			zmsg_destroy(&request);
			return msg;
		}
		else 
		if (--retries_left){
			if (self->verbose)
				zclock_log("W: no reply, reconnecting...");
			s_mdcli_connect_to_broker(self);
		}
		else {
			if (self->verbose)
				zclock_log("W: permanent error, abandoning");
			break;
		}
	}
	if (zctx_interrupted)
		printf("W: interrupt received, killing client...\n");
	zmsg_destroy(&request);
	return NULL;
}





