#ifndef PROCESSPOOL_H
#define PROCESSPOOL_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/stat.h>

class process
{
public:
	process() : m_pid(-1){}
public:
	pid_t m_pid;
	int m_pipefd[2];	
};

template< typename T >
class processpool
{
private:
	processpool(int listenfd, int process_number = 8);
public:
	static processpool< T >* create(int listenfd, int process_number = 8){
		if (!m_instance){
			m_instance = new processpool<T>(listenfd, process_number);
		}
		return m_instance;
	}
	~processpool()
	{
		delete [] m_sub_process;
	}
	void run();

private:
	void setup_sig_pipe();
	void run_parent();
	void run_child();
};
