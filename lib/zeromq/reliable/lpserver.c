#include "../zhelpers.h"

int main (void)
{
	srandom((unsigned)time(NULL));

	void *context = zmq_ctx_new();
	void *server = zmq_socket(context, ZMQ_REP);
	zmq_bind(server, "tcp://127.0.0.1:5555");

	int cycles = 0;
	while (1){
		char *request = s_recv(server);
		cycles++;

		if (cycles > 3 && randof(3) == 0){
			printf("I: simulating a crash\n");
			break;
		}
		else 
		if (cycles > 3 && randof(3) == 0){
			printf("I: simulating CPU overload\n");
			sleep(2);
		}
		printf("I: normal request (%s)\n", request);
		sleep(1);
		s_send(server, request);
		free(request);
	}
	zmq_close(server);
	zmq_ctx_destroy(context);
	return 0;
}