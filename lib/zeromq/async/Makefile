CC = gcc -std=gnu99
CFLAGS = -g -O2 -Wall -Wextra

USR_LOCAL = /usr/local

TARGETS = async_server
TARGETS_C_FILE = $(TARGETS).c

CLEANUP = rm -f $(TARGETS) *.o

all: $(TARGETS)

clean: 
	$(CLEANUP)

LDFLAGS = -L/usr/local/zeromq/lib -L/usr/local/czmq/lib

LIBS = -lzmq -lczmq

ZMQ_INCLUDES = -I/usr/local/zeromq/include -I/usr/local/czmq/include

$(TARGETS): $(TARGETS_C_FILE)
	$(CC) $(CFLAGS) $(ZMQ_INCLUDES) $(LDFLAGS) $(LIBS) $^ -o $@