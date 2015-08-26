#include "czmq.h"

static void *
client_task (void *args)
{
	zctx_t *ctx = zctx_new();
	void *client = zsocket_new(ctx, ZMQ_DEALER);

	char identity[10];
}