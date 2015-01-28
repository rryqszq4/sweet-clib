#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


char* str_delchar(char* s1, char *s2)
{
	char *p;
    static char res[1024];
    memset(res, 0, sizeof(res));
    p = res;
    while (*s1 != '\0'){
		if (*s1 == *s2){
		}else {
			*p = *s1;
			p++;
		}
		s1++;
    }
    return res;
}

int main (int argc, char **argv)
{
	char *str = "abc\"ASDF123\"";
	str = str_delchar(str,"\"");
	printf("%s\n",str);
}
