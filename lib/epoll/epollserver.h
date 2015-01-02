#ifndef EPOLLSERVER_H
#define EPOLLSERVER_H

#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <pthread.h>

#define _MAX_SOCKFD_COUNT 65535

class EpollServer {
	public:
		EpollServer();
		~EpollServer();

		bool InitServer(const char* chIp, int iPort);
		void Listen();
		static void ListenThread(void* lpVoid);
		void Run();

	private:
		int	m_iEpollFd;
		int m_isock;
		pthread_t m_ListenThreadId; // 监听线程句柄

};
#endif
