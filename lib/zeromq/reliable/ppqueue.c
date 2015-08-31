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
}






