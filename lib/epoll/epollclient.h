#ifndef _EPOLL_CLIENT_H
#define _EPOLL_CLIENT_H
#define _MAX_SOCKFD_COUNT 65536

#include <iostream>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <string>

using namespace std;

typedef enum _EPOLL_USER_STATUS_EM {
	FREE = 0,
	CONNECT_OK = 1, // 连接成功
	SEND_OK = 2, // 发送成功
	RECV_OK = 3, // 发送陈功
}EPOLL_USER_STATUS_EM;

struct UserStatus
{
	EPOLL_USER_STATUS_EM iUserStatus; // 用户状态
	int iSockFd; //用户状态关联的socketfd
	char cSendbuff[1024]; //发送的数据内容
	int iBuffLen; //发送数据的长度
	unsigned int uEpollEvents; 
}

class EpollClient {
	public:
		EpollClient(int iUserCount, const char* pIP, int iPort);
		~EpollClient();
		int RunFun();
	
	private:
		int ConnectToServer(int iUserId, const char *pServerIp, unsigned short uServerPort);
		int SendToServerData(int iUserId);
		int RecvFromServer(int iUserid, char *pRecvBuff, int iBuffLen);
		bool CloseUser(int iUserId);
		bool DelEpoll(int iSockFd);

	private:
		int m_iUserCount; //用户数量
		struct UserStatus *m_pAllUserStatus; //用户状态组 
		int m_iEpollFd; //需要创建epollfd
		int m_iSockFd_UserId[_MAX_SOCKFD_COUNT];//将用户ID和socketid关联
		int m_iPort; //端口号
		char m_ip[100];//IP地址
};

#endif
