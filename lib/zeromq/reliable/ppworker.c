#include "czmq.h"
#define HEARTBEAT_LIVENESS 3
#define HEARTBEAT_INTERVAL 1000
#define INTERVAL_INIT	1000
#define INTERVAL_MAX	32000

#define PPP_READY	"\001"
#define PPP_HEARTBEAT "\002"

static void *
s_worker_socket(zctx_t *ctx){
	void *worker = zsocket_new(ctx, ZMQ_DEALER);
	zsocket_connect(worker, "tcp://127.0.0.1:5556");

	printf("I: worker ready\n");
	zframe_t *frame = zframe_new(PPP_READY, 1);
	zframe_send(&frame, worker, 0);

	return worker;
}

int main (void)
{
	zctx_t *ctx = zctx_new();
	void *worker = s_worker_socket(ctx);

	size_t liveness = HEARTBEAT_LIVENESS;
	size_t interval = INTERVAL_INIT;

	uint64_t heartbeat_at = zclock_time() + HEARTBEAT_INTERVAL;

	srandom((unsigned)time(NULL));
	int cycles = 0;
	while (true){
		zmq_pollitem_t items[] = {{worker, 0, ZMQ_POLL, 0}};
		int rc = zmq_poll(items, 1, HEARTBEAT_INTERVAL * ZMQ_POLL_MSEC);
		if (rc == -1)
			break;

		if (items[0].revents & ZMQ_POLLIN){
			zmsg_t *msg = zmsg_recv(worker);
			if (!msg)
				break;

			if (zmsg_size(msg) == 3){
				cycles++;
				if (cycles > 3 && randof(5) == 0){
					printf("I:simulating a crash\n");
					zmsg_destroy(&msg);
					break;
				}
				else
				if (cycles > 3 && randof(5) == 0){
					printf("I: simulating CPU overload\n");
					sleep(3);
					if (zctx_interrupted)
						break;
				}
				printf("I: normal reply\n");
				zmsg_send(&msg, worker);
				liveness = HEARTBEAT_LIVENESS;
				sleep(1);
				if (zctx_interrupted)
					break;
			}
			else
			if (zmsg_size(msg) == 1){
				zframe_t *frame = zmsg_first(msg);
				if (memcmp(zframe_data(frame),PPP_HEARTBEAT, 1) == 0)
					liveness = HEARTBEAT_LIVENESS;
				else {
					printf("E: invalid message\n");
					zmsg_dump(msg);
				}
				zmsg_destroy(&msg);
			}
			else {
				printf("E: invalid message\n");
				zmsg_dump(msg);
			}
			interval = INTERVAL_INIT;
		}
		else 
		if (--liveness == 0){
			printf("W: heartbeat failure, can't reach queue\n");
			printf("W: reconnecting in %zd msec...\n", interval);
			zclock_sleep(interval);

			if (interval < INTERVAL_MAX)
				interval *= 2;
			zsocket_destroy(ctx, worker);
			worker = s_worker_socket(ctx);
			liveness = HEARTBEAT_LIVENESS;
		}

		if (zclock_time() > heartbeat_at){
			heartbeat_at = zclock_time() + HEARTBEAT_INTERVAL;
			printf("I: worker heartbeat\n");
			zframe_t * frame = zframe_new(PPP_HEARTBEAT, 1);
			zframe_send(&frame, worker, 0);
		}
	}
	zctx_destroy(&ctx);
	return 0;
}









