#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#define BUFF_SIZE	512

int
main (void)
{
	int fd_r;
	int fd_w;
	int res;
	int n;
	char buf[BUFF_SIZE+1];
	char *path_r = "test.txt";
	char *path_w = "write.txt";
	fd_r = open(path_r, O_CREAT | O_RDWR, 0666);
	fd_w = open(path_w, O_CREAT | O_RDWR, 0666);
	//fd_w = open(path_w, O_CREAT | O_RDWR | O_SYNC, 0666);

	while ((n = read(fd_r, buf, BUFF_SIZE)) > 0){
		if (write(fd_w, buf, n) != n){
			printf("write error\n");
		}
		res = fdatasync(fd_w);
	}	
		//res = fdatasync(fd_w);

	close(fd_r);
	close(fd_w);

	return 0;
}
