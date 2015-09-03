#include "mdcliapi2.h"

struct _mdcli_t
{
	zctx_t *ctx;	//上下文
	char *broker;
	void *client;
	int verbose;	//打印标准输出
	int timeout;
	//int retries;
};

void s_mdcli_connect_to_broker(mdcli_t *self)
{
	if (self->client)
		zsocket_destroy(self->ctx, self->client);
	self->client = zsocket_new(self->ctx, ZMQ_DEALER);
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
	//self->retries = 3;

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

int
mdcli_send(mdcli_t *self, char *service, zmsg_t **request_p)
{
	assert(self);
	assert(request_p);
	zmsg_t *request = *request_p;

	zmsg_pushstr(request, service);
	zmsg_pushstr(request, MDPC_CLIENT);
	zmsg_pushstr(request, "");
	if (self->verbose){
		zclock_log("I: send request to '%s' service:", service);
		zmsg_dump(request);
	}
	zmsg_send(&request, self->client);
	return 0;

}

zmsg_t *
mdcli_recv(mdcli_t *self)
{
	assert(self);
	
	zmq_pollitem_t items [] = {
		{self->client, 0, ZMQ_POLLIN, 0}
	};

	int rc = zmq_poll(items, 1, self->timeout * ZMQ_POLL_MSEC);
	if (rc == -1)
		return NULL;

	if (items[0].revents & ZMQ_POLLIN){
		zmsg_t *msg = zmsg_recv(self->client);
		if (self->verbose){
			zclock_log("I: received reply:");
			zmsg_dump(msg);
		}

		assert(zmsg_size(msg) >= 4);

		zframe_t *empty = zmsg_pop(msg);
		assert(zframe_streq(empty, ""));
		zframe_destroy(&empty);

		zframe_t *header = zmsg_pop(msg);
		assert(zframe_streq(header, MDPC_CLIENT));
		zframe_destroy(&header);

		zframe_t *service = zmsg_pop(msg);
		zframe_destroy(&service);

		return msg;
	}
	
	if (zctx_interrupted)
		printf("W: interrupt received, killing client...\n");
	
	return NULL;
}





