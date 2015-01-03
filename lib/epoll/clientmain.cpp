#include "epollclient.h"

int main(int argc, char *argv[])
{
	EpollClient *pCEpollClient = new EpollClient(2,"127.0.0.1",8021);
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
