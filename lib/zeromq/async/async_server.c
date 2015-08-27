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

void *server_task(void *args)
{
	zctx_t *ctx = zctx_new();
	void *frontend = zsocket_new(ctx, ZMQ_ROUTER);
	zsocket_bind(frontend, "tcp://127.0.0.1:5570");

	void *backend = zsocket_new(ctx, ZMQ_DEALER);
	zsocket_bind(backend, "inproc://backend");

	int thread_nbr;
	for (thread_nbr = 0; thread_nbr < 5; thread_nbr++){
		zthread_fork(ctx, server_worker, NULL);
	}

	zmq_proxy(frontend, backend, NULL);

	zctx_destroy(&ctx);
	return NULL;
}


static void 
server_worker (void *args, zctx_t *ctx, void *pipe)
{
	void *worker = zsocket_new(ctx, ZMQ_DEALER);
	zsocket_connect(worker, "inproc://backend");

	while (true){
		zmsg_t *msg = zmsg_recv(worker);
		zframe_t *indentity = zmsg_pop(msg);
		zframe_t *content = zmsg_pop(msg);
		assert(content);
		zmsg_destroy(&msg);

		int reply, replies = randof(5);
		for (reply = 0; reply < replies; reply++){
			zclock_sleep(randof(1000)+1);
			zframe_send(&identity, worker, ZFRAME_REUSE + ZFRAME_MORE);
			zframe_send(&content, worker, ZFRAME_REUSE);
		}
		zframe_destroy(&identity);
		zframe_destroy(&content);
	}


}

int main (void)
{
	zthread_new(client_task, NULL);
	zthread_new(client_task, NULL);
	zthread_new(client_task, NULL);
	zthread_new(server_task, NULL);
	zclock_sleep(5 * 1000);
	return 0;
}

