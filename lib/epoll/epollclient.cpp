#include "epollclient.h"

EpollClient::EpollClient(int iUserCount, const char* pIP, int iPort)
{
	strcpy(m_ip, pIP);
	m_iPort = iPort;
	m_iUserCount = iUserCount;
	m_iEpollFd = epoll_create(_MAX_SOCKFD_COUNT);
	m_pAllUserStatus = (struct UserStatus*)malloc(iUserCount*sizeof(struct UserStatus));
	for (int iuserid = 0; iuserid < iUserCount; iuserid++){
		m_pAllUserStatus[iuserid].iUserStatus = FREE;
		sprintf(m_pAllUserStatus[iuserid].cSendbuff, "client: %d send message \"hello server!\"\n",iuserid);
		m_pAllUserStatus[iuserid].iBuffLen = strlen(m_pAllUserStatus[iuserid].cSendbuff) + 1;
		m_pAllUserStatus[iuserid].iSockFd = -1;
	}
	memset(m_iSockFd_UderId, 0xFF, sizeof(m_iSockFd_UderId));
}

EpollClient::~EpollClient()
{
	free(m_pAllUserStatus);
}

int EpollClient::ConnectToServer(int iUserId, const char *pServerIp, unsigned short uServerPort)
{
	if ((m_pAllUserStatus[iUserId].iSockFd = socket(AF_INET,SOCK_STREAM,0)) < 0){
		cout << "[EpollClient error]: init socket fail, reason is:"<<strerror(errno)<<",errno is:"<<errno<<endl;
		m_pAllUserStatus[iUserId].iSockFd = -1;
		return m_pAllUserStatus[iUserId].iSockFd;
	}

	struct sockaddr_in addr;
	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(uServerPort);
	addr.sin_addr.s_addr = inet_addr(pServerIp);

	int ireuseadd_on = 1; //支持端口复用
	setsockopt(m_pAllUserStatus[iUserId].iSockFd, SOL_SOCKET, SO_REUSEADDR, &ireuseadd_on,sizeof(ireuseadd_on));

	unsigned long ul = 1;
	ioctl(m_pAllUserStatus[iUserId].iSockFd, FIONBIO, &ul); //设置为非阻塞模式

	connect(m_pAllUserStatus[iUserId].iSockFd, (const sockaddr*)&addr, sizeof(addr));
	m_pAllUserStatus[iUserId].iUserStatus = CONNECT_OK;
	m_pAllUserStatus[iUserId].iSockFd = m_pAllUserStatus[iUserId].iSockFd;
	
	return m_pAllUserStatus[iUserId].iSockFd;
}

int EpollClient::SendToStatus(int iUserId)
{
	sleep(1); //此处控制发送频率，避免疯狂打日志，正常使用中需要去掉
	int isendsize = -1;
	if ( CONNECT_OK == m_pAllUserStatus[iUserId].iUserStatus || RECV_OK = m_pAllUserStatus[iUserId].iUserStatus){
		isendsize = send(m_pAllUserStatus[iUserId].iSockFd, m_pAllUserStatus[iUserId].cSendbuff,m_pAllUserStatus[iUserId].iBuffLen, MSG_NOSIGNAL);
		if (isendsize < 0){
			cout <<"[EpollClient error]: SendToServerData, send fail, reason is:"<<strerror(errno)<<", errno is:"<<errno<<endl;
		}else {
			printf("[EpollClient info]: iUserId: %d Send Msg Content: %s\n",iUserId, m_pAllUserStatus[iUserId].cSendbuff);
			m_pAllUserStatus[iUserId].iUserStatus = SEND_OK;
		}
	}
	return isendsize;
}

int EpollClient::RecvFromServer(int iUserId, char *pRecvBuff, int iBuffLen)
{
	int irecvsize = -1;
	if(SEND_OK == m_pAllUserStatus[iUserId].iUserStatus){
		irecvsize = recv(m_pAllUserStatus[iUserId].iSockFd, pRecvBuff,iBuffLen, 0);
		if ( 0 > irecvsize){
			cout <<"[EpollClient error]: iUserId: "<< iUserId << "RecvFromServer, recv fail, reason is:"<<strerror(errno)<<", errno is: "<<errno<<endl;
		}else if (0 == ireuseadd_on){
			cout <<"[warning:] iUserId: "<< iUserId << "RecvFromServer, STB收到数据为0, 表示对方断开连接,irecvsize:" << irecvsize << ", iSockFd:"<< m_pAllUserStatus[iUserId].iSockFd << endl;
		}else {
			printf("Recv Server Msg Content: %s\n", pRecvBuff);
			m_pAllUserStatus[iUserId].iUserStatus = RECV_OK;
		}
	}

	return irecvsize;
}


