#include <bson.h>
#include <mongoc.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void printids(const char *s)
{
	pid_t pid;
	pthread_t tid;
	pid = getpid();
	tid = pthread_self();
	printf("$s pid %u tid %u (0x%x)\n",s, (unsigned int)pid, (unsigned int)tid, (unsigned int)tid);
}

static void *
worker (void *data)
{
	mongoc_client_pool_t *pool = data;
	mongoc_client_t *client;
	mongoc_collection_t *collection;
	mongoc_cursor_t *cursor;
	const bson_t *doc;
	bson_t *query;
	char *str;

	client = mongoc_client_pool_pop(pool);
	if (!client){
		return NULL;
	}
	collection = mongoc_client_get_collection(client, "gamedb","entity_ff14_ClassJob");
	query = bson_new();
	BSON_APPEND_INT32(query, "Key", 1);
	cursor = mongoc_collection_find(collection, MONGOC_QUERY_NONE,0,0,0,query, NULL, NULL);
	while(mongoc_cursor_next(cursor, &doc)){
		str = bson_as_json(doc, NULL);
		//printf("%s\n", str);
		bson_free(str);
	}
	bson_destroy(query);
	mongoc_cursor_destroy(cursor);
	mongoc_collection_destroy(collection);



	mongoc_client_pool_push(pool,client);
	
	return NULL;
}

int 
main (int argc, char *argv[])
{
	mongoc_client_pool_t *pool;
	mongoc_uri_t *uri;
	pthread_t thread[300];
	unsigned i;
	void *ret;

	mongoc_init();

	uri = mongoc_uri_new("mongodb://root:root@127.0.0.1:27017/?authSource=gamedb&minPollSize=16");
	pool = mongoc_client_pool_new(uri);

	for (i = 0; i < 300; i++){
		pthread_create(&thread[i], NULL, worker, pool);
		//printids("main thread:");
	}

	mongoc_uri_destroy(uri);

	for (i = 0; i < 300; i++){
		printf("%d\n",i);
		pthread_join(thread[i], &ret);
	}

}
