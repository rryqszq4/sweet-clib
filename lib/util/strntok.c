#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *__strntok;

char* strntok(char *s1, char *s2, int n)
{
	char *p;
	char *pp;
	char *swap;
	char tmp[n+1];
	static char res[1024];
	int i = 0;

	if (s1 == NULL){
		return NULL;
	}

	memset(tmp, 0, sizeof(tmp));
	memset(res, 0, sizeof(res));
	p = tmp;
	pp = res;
	
	while (*s1 !='\0'){
		if (i == n){
			i = 0;
			//printf("tmp: %s\n", tmp);
			//printf("res: %s\n", res);
			//printf("pp: %s\n", pp);
			__strntok = s1;
			return res;
		}
		if (*s1 == *s2 || i>0){
			i++;
			*p = *s1;
			p++;
		}else {
			*pp = *s1;
			pp++;
		}
		//*p = *s1;
		//p++;
		s1++;
		//printf("s1: %s\n",s1);
		//printf("p: %s\n",p);
		//printf("tmp: %s\n",tmp);
	}
	//printf("res: %s\n",res);
	__strntok = NULL;
	return res;
}

int main(int artc, char *argv[])
{
	char str[] = "3abcasdfasdf<br/>abc<br/>123";
	
	char *ptr;
	ptr = strntok(str, "<br/>", 5);
	while(ptr != NULL){
		printf("%s\n", ptr);
		//printf("%s\n", __strntok);
		ptr = strntok(__strntok, "<br/>", 5);	
	}
}
