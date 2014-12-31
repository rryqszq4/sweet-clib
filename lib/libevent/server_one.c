#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#include "event2/bufferevent.h"
#include "event2/buffer.h"
#include "event2/listener.h"

static const char message[] = "hello,world\n";
static const int port = 9995;

static void listener_cb(struct evconnlistener *, evutil_socket_t, struct sockaddr *, int socklen, void*);
static void signal_cb(evutil_socket_t, short, void*);

int main(int argc,char **argv){
	struct event_base *base;
	struct evconnlistener *listener;
	struct event *signal_event;

	struct sockaddr_in sin; //ipv4

	// 初始化libevent, libevent2推荐使用event_base_new(), 因为fork后更好用
	base = event_base_new();
	if (!base){
		fprintf(stderr, "could not initialize libevent\n");
		return 1;
	}

	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port); //转换整形端口为网络字节序

	// 绑定到ip:port监听
	listener = evconnlistener_new_bind(base, listener_cb, (void*)base,LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE,-1,(struct sockaddr*)&sin, sizeof(sin));

	if (!listener){
		fprintf(stderr,"could not create a listener\n");
		return 1;
	}

	// 添加一个处理信号的事件
	signal_event = evsignal_new(base, SIGINT, signal_cb, (void*)base);

	if (!signal_event || event_add(signal_event, NULL) < 0){
		fprintf(stderr,"could not create/add a signal event\n");
		return 1;
	}

	// 运行
	event_base_dispatch(base);

	// 释放已经分配的资源（上面函数中用过new 的）
	evconnlistener_free(listener);
	event_free(signal_event);
	event_base_free(base);

	printf("done\n");
	return 0;
}


// 监听到有人连接，处理
static void listener_cb(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *sa, int socklen, void *user_data){
	struct event_base *base = user_data;
	struct bufferevent *bev;

	bev = bufferevent_socket_new(base,fd,BEV_OPT_CLOSE_ON_FREE);
	if (!bev){
		fprintf(stderr, "error constructing bufferevent");
		event_base_loopbreak(base);
		return ;
	}
	
	// 向客户端发送消息 message
	bufferevent_write(bev,message,strlen(message));

}

static void signal_cb(evutil_socket_t sig,short events,void *user_data){
	struct event_base *base = user_data;
	struct timeval delay = {2, 0};

	printf("caught an interrupt singal; exiting cleanly in two seconds.\n");

	//结束
	event_base_loopexit(base,&delay);
}
