#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	char str[]="a1b2,cd,ef";
	char *ptr;
	char *p;
	printf("berfore strtok: str=%s\n", str);
	printf("begin:\n");
	ptr = strtok_r(str, ",", &p);
	while(ptr != NULL){
		printf("str=%s\n",str);
		printf("ptr=%s\n",ptr);
		ptr = strtok_r(NULL, ",", &p);
	}
	return 0;
}
