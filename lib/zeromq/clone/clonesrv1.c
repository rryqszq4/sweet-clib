/**
gcc -o clonesrv1 clonesrv1.c -I/usr/local/czmq/include -I/usr/local/zeromq/include \
 -L/usr/local/zeromq/lib -lzmq -L/usr/local/czmq/lib -lczmq
*/

#include "kvsimple.c"

int main (void)
{
	zctx_t *ctx = zctx_new();
	void *publisher = zsocket_new(ctx, ZMQ_PUB);
	zsocket_bind(publisher, "tcp://127.0.0.1:5556");
	zclock_sleep(200);

	zhash_t *kvmap = zhash_new();
	int64_t sequence = 0;
	srandom((unsigned) time (NULL));

	while (!zctx_interrupted){
		kvmsg_t *kvmsg = kvmsg_new(++sequence);
		kvmsg_fmt_key(kvmsg, "%d", randof(10000));
		kvmsg_fmt_body(kvmsg, "%d", randof(1000000));
		kvmsg_send(kvmsg, publisher);
		kvmsg_store(&kvmsg, kvmap);
	}
	printf("Interrupted\n%d message out\n", (int)sequence);
	zhash_destroy(&kvmap);
	zctx_destroy(&ctx);
	return 0;
}