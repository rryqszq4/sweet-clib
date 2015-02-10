#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include "list.h"
#include "queue.h"

typedef struct _threadworker {
	void *(*process)(void *arg);
	void *arg;
} threadworker;

typedef struct _threadpool {
	pthread_mutex_t queue_lock;
	pthread_cond_t queue_ready;
	
	Queue work_queue;
	
	int shutdown;
	pthread_t *thread_ids;
	int max_thread_num;
	int cur_queue_size;

} threadpool;

static threadpool *pool = NULL;

void threadpool_init(int max_thread_num);
int threadpool_destroy();
int threadpool_add_worker(void *(*process)(void *arg), void *arg);
void *threadpool_handle(void *arg);
void *myprocess(void *arg);
#endif
