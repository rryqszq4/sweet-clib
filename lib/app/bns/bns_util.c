#include "bns_util.h"

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
		if (*s1 == *s2  || i>0){
			if (i == 0){
				s1++;s2++;
				if (*s1 != *s2){
					i--;s1--;s2--;
					*pp = *s1;
					pp++;
				}else {
					s1--;s2--;
				}
			}
			i++;
			//*p = *s1;
			//p++;
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