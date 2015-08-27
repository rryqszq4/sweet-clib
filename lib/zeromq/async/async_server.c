#include "czmq.h"

static void *
client_task (void *args)
{
	zctx_t *ctx = zctx_new();
	void *client = zsocket_new(ctx, ZMQ_DEALER);

	char identity[10];
	sprintf(identity, "%04X-%04X", randof(0x10000), randof(0x10000));
	zsocket_set_identity(client, identity);
	zsocket_connect(client, "tcp://127.0.0.1:5570");

	zmq_pollitem_t items[] = {{client, 0, ZMQ_POLLIN, 0}};
	int request_nbr = 0;
	while(true){
		int centitick;
		for (centitick = 0; centitick < 100; centitick++){
			zmq_poll (items,1, 10 *ZMQ_POLL_MSEC);
			if (items[0].revents & ZMQ_POLLIN){
				zmsg_t *msg = zmsg_recv(client);
				zframe_print(zmsg_last(msg), identity);
				zmsg_destroy(&msg);
			}
		}
		zstr_sendf(client, "request #%d", ++request_nbr);
	}

	zctx_destroy(&ctx);
	return NULL;
}