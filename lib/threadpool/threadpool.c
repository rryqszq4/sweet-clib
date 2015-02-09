#include "threadpool.h"

void threadpool_init(int max_thread_num)
{
	pool = (threadpool *)malloc(sizeof(threadpool));

	pthread_mutex_init(&(pool->queue_lock), NULL);
	pthread_cond_init(&(pool->queue_ready), NULL`);
	
	queue_init(pool->work_queue,free);

	pool->max_thread_num = max_thread_num;
	pool->cur_queue_size = 0;
	pool->shutdown = 0;
	
	pool->thread_ids = (pthread_t *)malloc(max_thread_num * sizeof(pthread_t));
	int i;
	for (i=0; i < max_thread_num; i++){
		pthread_create(&(pool->thread_ids[i]), NULL , thread_handle, NULL);
	}

}

int pool_add_worker(void *(*process)(void *arg), void *arg)
{
	threadworker *new_worker = (threadworker *)malloc(sizeof(threadworker));
	new_worker->process = process;
	new_worker->arg = arg;

	pthread_mutex_lock(&(pool->queue_lock));
	queue_enqueue(pool->work_queue , new_worker);
	pool->cur_queue_size++;
	pthread_mutex_unlock(&(pool->queue_lock));
	
	pthread_cond_signal(&(pool->queue_ready));
	return 0;
}

int pool_destroy()
{
	if (pool->shutdown)
		return -1;
	pool->shutdown = 1;

	pthread_cond_broadcast(&(pool->queue_ready));

	int i;
	for(i=0; i < pool->max_thread_num; i++){
		pthread_join(pool->thread_ids[i], NULL);
	}
	free(pool->thread_ids);
	
	while (queue_peek(pool->work_queue) != NULL){
		queue_dequeue(pool->work_queue, queue_peek(pool->work_queue));
	}

	pthread_mutex_destroy(&(pool->queue_lock));
	pthread_cond_destroy(&(pool->queue_ready));

	free(pool);
	pool = NULL;
	return 0;
}

void *thread_handle(void *arg)
{
	printf("starting thread 0x%x\n", pthread_self);

}












