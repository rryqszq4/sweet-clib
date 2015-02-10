#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <assert.h>

#include "list.h"
#include "queue.h"
#include "threadpool.h"

int main(int argc, char *argv[])
{
	threadpool_init(3);
	
	int *workingnum = (int *)malloc(sizeof(int)*10);
	int i;
	for (i=0; i < 10; i++){
		workingnum[i] = i;
		threadpool_add_worker(myprocess, &workingnum[i]);
	}
	
	sleep(5);
	threadpool_destroy();

	free(workingnum);
	return 0;
}
