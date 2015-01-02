#include <iostream>
#include "epollserver.h"

using namespace std;

int main(){
	EpollServer theApp;
	theApp.InitServer("127.0.0.1",8021);
	theApp.Run();

	return 0;
}
