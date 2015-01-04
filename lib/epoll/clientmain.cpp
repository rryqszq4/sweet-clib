#include "epollclient.h"

int main(int argc, char *argv[])
{
	if (argc < 2){
		fprintf(stderr, "err: no number\n");
		exit(1);
	}
	EpollClient *pCEpollClient = new EpollClient(atoi(argv[1]),"127.0.0.1",8021);
	if (NULL == pCEpollClient){
		cout<<"[epollclient error]:main init"<<"Init EpollClient fail"<<endl;
	}

	pCEpollClient->RunFun();

	if (NULL != pCEpollClient){
		delete pCEpollClient;
		pCEpollClient = NULL;
	}

	return 0;
}
