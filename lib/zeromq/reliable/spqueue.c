#include "czmq.h"

#define WORKER_REPLY "\001"

int main(void)
{
	zctx_t *ctx = zctx_new();
	void *frontend = zsocket_new(ctx, ZMQ_ROUTER);
	void *backend = zsocket_new(ctx, ZMQ_ROUTER);
	zsocket_bind(frontend, "tcp://127.0.0.1:5555");
	zsocket_bind(backend, "tcp://127.0.0.1:5556");

	zlist_t * workers = zlist_new();

	while (true){
		zmq_pollitem_t items [] = {
			{backend, 0, ZMQ_POLLIN, 0},
			{frontend, 0, ZMQ_POLLIN, 0}
		};

		int rc = zmq_poll(items, zlist_size(workers)?2:1, -1);
		if (rc == -1)
			break;

		if (items[0].revents & ZMQ_POLLIN){
			zmsg_t *msg = zmsg_recv(backend);
			if (!msg)
				break;
			zframe_t *identity = zmsg_unwrap(msg);
			zlist_append(workers, identity);

			zframe_t *frame = zmsg_first(msg);
			if (memcmp(zframe_data(frame), WORKER_REPLY, 1) == 0)
				zmsg_destroy(&msg);
			else 
				zmsg_send(&msg, frontend);
		}
		if (items[1].revents & ZMQ_POLLIN){
			zmsg_t *msg = zmsg_recv(frontend);
			if (msg){
				zmsg_wrap(msg, (zframe_t *)zlist_pop(workers));
				zmsg_send(&msg, backend);
			}
		}
	}

	while (zlist_size(workers)){
		zframe_t *frame = (zframe_t *)zlist_pop(workers);
		zframe_destroy(&frame);
	}
	zlist_destroy(&workers);
	zctx_destroy(&ctx);
	return 0;
}