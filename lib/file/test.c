#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int
main (void)
{
	int fd;
	int res;
	char *path = "test.txt";
	fd = open(path, O_CREAT | O_RDWR | O_APPEND, 0666);

	char buf[1024*1024+1] = "abc";
	write(fd, buf, 1024*1024);

	res = fsync(fd);
	printf("fdatasync res=%d\n", res);


	close(fd);

	return 0;
}
