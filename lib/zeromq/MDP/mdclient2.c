/**
gcc -o mdclient2 mdclient2.c -I/usr/local/czmq/include -I/usr/local/zeromq/include \
-L/usr/local/zeromq/lib -lzmq -L/usr/local/czmq/lib -lczmq
*/

#include "mdcliapi2.c"

int main (int argc, char *argv[])
{
	int verbose = (argc > 1 && streq(argv[1], "-v"));
	mdcli_t *session = mdcli_new("tcp://127.0.0.1:5555", verbose);

	int count;
	for (count = 0; count < 100000; count++){
		zmsg_t *request = zmsg_new();
		zmsg_pushstr(request, "Hello world");
		mdcli_send(session, "echo", &request);
	}

	for (count = 0; count < 100000; count++){
		zmsg_t *reply = mdcli_recv(session);
		if (reply)
			zmsg_destroy(&reply);
		else 
			break;
	}
	printf("%d replies receivde\n", count);
	mdcli_destroy(&session);
	return 0;
}