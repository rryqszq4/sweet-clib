#include "czmq.h"

#define WORKER_READY "\001"

int main (void)
{
	zctx_t *ctx = zctx_new();
	void *worker = zsocket_new(ctx, ZMQ_REQ);

	srandom((unsigned)time(NULL));
	char identity[10];
	sprintf(identity, "%04X-%04X", randof(0x10000), randof(0x10000));
	zmq_setsockopt(worker, ZMQ_IDENTITY, identity, strlen(identity));
	zsocket_connect(worker, "tcp://127.0.0.1:5556");

	printf("I: (%s) worker ready\n", identity);
	zframe_t *frame = zframe_new(WORKER_READY, 1);
	zframe_send(&frame, worker, 0);

	int cycles = 0;
	while (true){
		zmsg_t *msg = zmsg_recv(worker);
		if (!msg)
			break;

		cycles++;
		if (cycles > 3 && randof(5) == 0){
			printf("I (%s) simulating a crash\n", identity);
			zmsg_destroy(&msg);
			break;
		}
		else 
		if (cycles > 3 && randof(5) == 0){
			printf("I: (%s) simulating CPU overload\n", identity);
			sleep(3);
			if (zctx_interrupted)
				break;
		}
		printf("I: (%s) normal reply\n", identity);
		sleep(1);
		zmsg_send(&msg, worker);
	}
	zctx_destroy(&ctx);
	return 0;
}