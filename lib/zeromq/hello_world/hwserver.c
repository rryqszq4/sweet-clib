/*
 * hello world server 
 * gcc -o hwserver hwserver.c -I /usr/local/zeromq/include -L/usr/local/zeromq/lib/ -lzmq
 * */

#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

int main (void)
{
	void *context = zmq_ctx_new();
	void *responder = zmq_socket(context, ZMQ_REP);
	int rc = zmq_bind(responder, "tcp://127.0.0.1:5555");
	assert(rc == 0);

	while (1){
		char buffer[10];
		zmq_recv(responder, buffer, 10, 0);
		printf("Received Hello\n");
		sleep(1);
		zmq_send(responder, "World", 5, 0);
	}

	return 0;
}
