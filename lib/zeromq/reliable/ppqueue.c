#include "czmq.h"
#define HEARTBEAT_LIVENESS 3
#define HEARTBEAT_INTERVAL 1000

#define PPP_READY "\001"
#define PPP_HEARTBEAT "\002"

typedef struct {
	zframe_t *identity;
	char *id_string;
	int64_t expiry;
} worker_t;

static worker_t *
s_worker_new (zframe_t *identity)
{
	worker_t *self = (worker_t *)zmalloc(sizeof(worker_t));
	self->identity = identity;
	self->id_string = zframe_strhex(identity);
	self->expiry = zclock_time() + HEARTBEAT_INTERVAL * HEARTBEAT_LIVENESS;
	return self;
}

static void
s_worker_destroy(worker_t **self_p)
{
	assert(self_p);
	if (*self_p){
		worker_t *self = self_p;
		zframe_destroy(&self->identity);
		free(self->id_string);
		free(self);
		*self_p = NULL;
	}
}

static void 
s_worker_ready(worker_t *self, zlist_t *workers)
{
	worker_t *worker = (worker_t *)zlist_first(workers);
	while (worker){
		if (streq(self->id_string, worker->id_string)){
			zlist_remove(workers, worker);
			s_worker_destroy(&worker);
			break;
		}
		worker = (worker_t *)zlist_next(workers);
	}
	zlist_append(workers, self);
}

static zframe_t *
s_workers_next(zlist_t *workers)
{
	worker_t *worker = zlist_pop(workers);
	assert(worker);
	zframe_t *frame = worker->identity;
	worker->identity = NULL;
	s_worker_destroy(&worker);
	return frame;
}

static void 
s_workers_purge(zlist_t *workers)
{
	worker_t *worker = (worker_t *)zlist_first(workers);
	while (worker){
		if (zclock_time() < worker->expiry)
			break;

		zlist_remove(workers, worker);
		s_worker_destroy(&worker);
		worker = (worker_t *)zlist_first(workers);
	}
}

int main(void)
{
	zctx_t *ctx = zctx_new();
	void *frontend = zsocket_new(ctx, ZMQ_ROUTER);
	void *backend = zsocket_new(ctx, ZMQ_ROUTER);
	zsocket_bind(frontend, "tcp://127.0.0.1:5555");
	zsocket_bind(backend, "tcp://127.0.0.1:5556");

	zlist_t *workers = zlist_new();

	uint64_t heartbeat_at = zclock_time() + HEARTBEAT_INTERVAL;

	while (true){
		zmq_pollitem_t items [] ={
			{backend, 0, ZMQ_POLLIN, 0},
			{frontend, 0, ZMQ_POLLIN, 0}
		};

		int rc = zmq_poll(items, zlist_size(worker)?2:1);
		if (rc == -1)
			break;

		if (items[0].revents & ZMQ_POLLIN){
			zmsg_t *msg = zmsg_recv(backend);
			if (!msg)
				break;

			zframe_t *identity = zmsg_unwrap(msg);
			worker_t *worker = s_worker_new(identity);
			s_worker_ready(worker, workers);

			if (zmsg_size(msg) == 1){
				zframe_t *frame = zmsg_first(msg);
				if (memcmp(zframe_data(frame), PPP_READY, 1) && memcmp(zframe_data(frame), PPP_HEARTBEAT, 1)){
					printf("E: invalid message from worker");
					zmsg_dump(msg);
				}
				zmsg_destroy(&msg);
			}
			else
				zmsg_send(&msg, frontend);
		}
		if (items[1].revents & ZMQ_POLLIN){
			zmsg_t *msg = zmsg_recv(frontend);
			if (!msg)
				break;
			zframe_t *identity = s_workers_next(workers);
			zmsg_prepend(msg, &identity);
			zmsg_send(&msg, backend);
		}

		if (zclock_time() >= heartbeat_at){
			worker_t *worker = (worker_t *)zlist_first(workers);
			while (worker){
				zframe_send(&worker->identity, backend, ZFRAME_REUSE + ZFRAME_MORE);
				zframe_t *frame = zframe_new(PPP_HEARTBEAT, 1);
				zframe_send(&frame, backend, 0);
				worker = (worker_t *)zlist_next(workers);
			}
			heartbeat_at = zclock_time() + HEARTBEAT_INTERVAL;
		}
	s_workers_purge(workers);
	}

	while (zlist_size(workers)){
		worker_t *worker = (worker_t *)zlist_pop(workers);
		s_worker_destroy(&worker);
	}
	zlist_destroy(&workers);
	zctx_destroy(&ctx);
	return 0;
}






