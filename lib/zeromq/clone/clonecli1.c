#include "kvsimple.c"

int main (void)
{
	zctx_t *ctx = zctx_new();
	void *updates = zsocket_new(ctx, ZMQ_SUB);
	zsocket_set_subscribe(updates, "");
	zsocket_connect(updates, "tcp://127.0.0.1:5556");

	zhash_t *kvmap = zhash_new();
	int64_t sequence = 0;

	while (true){
		kvmsg_t *kvmsg = kvmsg_recv(updates);
		if (!kvmsg)
			break;
		kvmsg_store(&kvmsg, kvmap);
		sequence++;
	}
	printf("Interrupted\n%d message in \n", (int) sequence);
	zhash_destroy(&kvmap);
	zctx_destroy(&ctx);
	return 0;
}