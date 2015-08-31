CC = gcc -std=gnu99
CFLAGS = -g -O2 -Wall -Wextra

USR_LOCAL = /usr/local

LP_CLIENT = lpclient
LP_SERVER = lpserver
SP_QUEUE = spqueue
SP_WORKER = spworker

CLEANUP = rm -f $(LP_CLIENT) $(LP_SERVER) $(SP_QUEUE) $(SP_WORKER) *.o

all: $(LP_CLIENT) $(LP_SERVER) $(SP_QUEUE) $(SP_WORKER)

clean: 
	$(CLEANUP)

LDFLAGS = -L/usr/local/zeromq/lib -L/usr/local/czmq/lib

LIBS = -lzmq -lczmq

ZMQ_INCLUDES = -I/usr/local/zeromq/include -I/usr/local/czmq/include

$(LP_CLIENT): $(LP_CLIENT).c
	$(CC) $(CFLAGS) $(ZMQ_INCLUDES) $(LDFLAGS) $(LIBS) $^ -o $@

$(LP_SERVER): $(LP_SERVER).c
	$(CC) $(CFLAGS) $(ZMQ_INCLUDES) $(LDFLAGS) $(LIBS) $^ -o $@

$(SP_QUEUE): $(SP_QUEUE).c
	$(CC) $(CFLAGS) $(ZMQ_INCLUDES) $(LDFLAGS) $(LIBS) $^ -o $@

$(SP_WORKER): $(SP_WORKER).c
	$(CC) $(CFLAGS) $(ZMQ_INCLUDES) $(LDFLAGS) $(LIBS) $^ -o $@