#include "mdwrkapi.c"

int main(int argc, char *argv[])
{
	int verbose = (argc > 1 && streq(argv[1], "-v"));
	mdwrk_t *session = mdwrk_new("tcp://127.0.0.1:5555", "echo", verbose);

	zmsg_t *reply = NULL;
	while (true){
		zmsg_t *request = mdwrk_recv(session, &reply);
		if (request == NULL)
			break;
		reply = request;
	}
	mdwrk_destroy(&session);
	return 0;
}