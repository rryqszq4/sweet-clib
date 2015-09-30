#ifndef _BUF_IO_H_INCLUDED_
#define _BUF_IO_H_INCLUDED_

#include <fcntl.h>

typedef struct _buffer_io buffer_io;

struct _buffer_io
{
	char *pathname;

};

#define O_READ_ONLY		O_RDONLY
#define O_WRITE_ONLY	O_WRONLY
#define O_READ_WRITE	O_RDWR 
#define O_APPEND

buffer_io *bufio_new();
void bufio_destroy(buffer_id **bufio);

#endif