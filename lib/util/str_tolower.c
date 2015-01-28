#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


char* str_tolower(char* s1)
{
	char *p;
	static char res[1024];
	memset(res, 0, sizeof(res));
	p = res;
	while (*s1 != '\0'){
		*p = *s1;
		*p = tolower(*p);
		//printf("%s\n",p);
		p++;
		s1++;
		//printf("%s\n",s1);
		//printf("%s\n",res);
	}
	return res;
}

int main (int argc, char **argv)
{
	char *str = "abcASDF123";
	str = str_tolower(str);
	printf("%s\n",str);
}
