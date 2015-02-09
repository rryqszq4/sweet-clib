#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <list.h>
#include <queue.h>
#include <pthread.h>

typedef struct _threadworker {
	void *(*process)(void *arg);
	void *arg;
} threadworker;

typedef struct _threadpool {
	pthread_mutext_t queue_lock;
	pthread_cond_t queue_ready;
	
	Queue* work_queue;
	
	int shutdown;
	pthread_t *thread_ids;
	int max_thread_num;
	int cur_queue_size;

} threadpool;

static threadpool *pool = NULL;

void threadpool_init(int max_thread_num);
int pool_destroy();
int pool_add_worker(void *(*process)(void *arg), void *arg);
void *thread_handle(void *arg);
#endif
