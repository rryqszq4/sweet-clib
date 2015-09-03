#include "czmq.h"

static void
client_task(void *args, zctx_t *ctx, void *pipe)
{
	void *client = zsocket_new(ctx, ZMQ_DEALER);
	zsocket_connect(client, "tcp://127.0.0.1:5555");
	printf("Setting up test...\n");
	zclock_sleep(100);

	int requests;
	int64_t start;

	printf("Synchronous round-trip test...\n");
	start = zclock_time();
	for (requests = 0; requests < 10000; requests++){
		zstr_send(client, "hello");
		char *reply = zstr_recv(client);
		free(reply);
	}
	printf(" %d calls/second\n", (1000 * 10000)/(int)(zclock_time() - start));

	printf("Asynchronous round-trip test...\n");
	start = zclock_time();
	for (requests = 0; requests < 100000; requests++)
		zstr_send(client, "hello");
	for (requests = 0; requests < 100000; requests++){
		char *reply = zstr_recv(client);
		free(reply);
	}
	printf(" %d calls/second\n", (1000 * 100000)/(int)(zclock_time() - start));
	zstr_send(pipe, "done");
}

static void *
worker_task(void *args)
{
	zctx_t *ctx = zctx_new();
	void *worker = zsocket_new(ctx, ZMQ_DEALER);
	zsocket_connect(worker, "tcp://127.0.0.1:5556");

	while (true){
		zmsg_t *msg = zmsg_recv(worker);
		zmsg_send(&msg, worker);
	}
	zctx_destroy(&ctx);
	return NULL;
}

static void *
broker_task(void *args)
{
	zctx_t *ctx = zctx_new();
	void *frontend = zsocket_new(ctx, ZMQ_DEALER);
	zsocket_bind(frontend, "tcp://127.0.0.1:5555");
	void *backend = zsocket_new(ctx, ZMQ_DEALER);
	zsocket_bind(backend, "tcp://127.0.0.1:5556");
	zmq_proxy(frontend, backend, NULL);
	zctx_destroy(&ctx);
	return NULL;
}

int main(void)
{
	zctx_t *ctx = zctx_new();
	void *client = zthread_fork(ctx, client_task, NULL);
	zthread_new(worker_task, NULL);
	zthread_new(broker_task, NULL);

	char *signal = zstr_recv(client);
	free(signal);

	zctx_destroy(&ctx);
	return 0;
}








