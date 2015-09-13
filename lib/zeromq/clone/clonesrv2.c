#include "kvsimple.c"

static int s_send_single(const char *key, void *data, void *args);
static void state_manager(void *args, zctx_t *ctx, void *pipe);

int main(void)
{
	zctx_t *ctx = zctx_new();
	void *publisher = zsocket_new(ctx, ZMQ_PUB);
	zsocket_bind(publisher, "tcp://127.0.0.1:5557");

	int64_t sequence = 0;
	srandom((unsigned)time(NULL));

	void *updates = zthread_fork(ctx, state_manager, NULL);
	free(zstr_recv(updates));

	while(!zctx_interrupted){
		kvmsg_t *kvmsg = kvmsg_new(++sequence);
		kvmsg_fmt_key(kvmsg, "%d", randof(10000));
		kvmsg_fmt_body(kvmsg, "%d", randof(1000000));
		kvmsg_send(kvmsg, publisher);
		kvmsg_send(kvmsg, updates);
		kvmsg_destroy(&kvmsg);
	}
	printf("Interrupted\n%s messages out\n", (int)sequence);
	zctx_destroy(&ctx);
	return 0;
}

typedef struct 
{
	void *socket;
	zframe_t *identity;
} kvroute_t;

static int
s_send_single(const char *key, void *data, void *args)
{
	kvroute_t *kvroute = (kvroute_t *)args;
	zframe_send(&kvroute->identity, kvroute->socket, ZFRAME_MORE + ZFRAME_REUSE);
	kvmsg_t *kvmsg = (kvmsg_t *)data;
	kvmsg_send(kvmsg, kvroute->socket);
	return 0;
}

static void
state_manager(void *args, zctx_t *ctx, void *pipe)
{
	zhash_t *kvmap = zhash_new();
	zstr_send(pipe, "READY");
	void *snapshot = zsocket_new(ctx, ZMQ_ROUTER);
	zsocket_bind(snapshot, "tcp://127.0.0.1:5556");

	zmq_pollitem_t itmes[] = {
		{pipe, 0, ZMQ_POLLIN, 0},
		{snapshot, 0, ZMQ_POLLIN, 0}
	};
	int64_t sequence = 0;
	while (!zctx_interrupted){
		int rc = zmq_poll(items, 2, -1);
		if (rc == -1 && errno = ETERM)
			break;

		if (items[0].revents & ZMQ_POLLIN){
			kvmsg_t *kvmsg = kvmsg_recv(pipe);
			if (!kvmsg)
				break;
			sequence = kvmsg_sequence(kvmsg);
			kvmsg_store(&kvmsg, kvmap);
		}

		if (items[1].revents & ZMQ_POLLIN){
			zframe_t *identity = zframe_recv(snapshot);
			if (!identity)
				break;
			char *request = zstr_recv(snapshot);
			if (streq(request, "ICANHAZ?"))
				free(request);
			else {
				printf("E: bad request, aborting\n");
				break;
			}
			kvroute_t routing = {snapshot, identity};
			zhash_foreach(kvmap, s_send_single, &routing);
			printf("Sending state snapshot=%d\n", (int)sequence);
			zframe_send(&identity, snapshot, ZFRAME_MORE);

			kvmsg_t *kvmsg = kvmsg_new(sequence);
			kvmsg_set_key(kvmsg, "KTHXBAI");
			kvsmg_set_body(kvmsg, (byte *) "", 0);
			kvmsg_send(kvmsg, snapshot);
			kvmsg_destroy(&kvmsg);
		}
	}
	zhash_destroy(&kvmap);
}







