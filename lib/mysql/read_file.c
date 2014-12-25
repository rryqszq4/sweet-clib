#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *my_csv(char *s1, char *s2){
	char *p;
	char *comma = ",";
	
	p = s2;
	while(*s1 != *comma){
		*p = *s1;
		p++;
		s1++;
	}
	if (*s1 == *comma){
		s1++;
	}
	//printf("%s\n",s2);
	//printf("%s\n",p);
	//printf("%s\n",s1);
	return s1;
}

int main(int argc, char *argv[]){
	FILE *fp;
	char strline[1024];
	char *ss1;
	char result[128];
	char *comma = ",";
	char tip[] = "|";
	int i,n;
	char temp[128];

	if (argc != 2){
		fprintf(stderr,"err: no file!\n");
		exit(1);
	}

	if ((fp = fopen(argv[1],"r")) == NULL){
		printf("error!");
		return -1;
	}

	i = 0;
	while(!feof(fp)){
		memset(strline,0,sizeof(strline));
		fgets(strline,1024,fp);
		/*if (i == 0){
			i++;
			continue;
		}*/
		//printf("%s\n",strline);
		
		n = 0;
		
		ss1 = strline;
		while(ss1 != NULL){
			memset(result,0,sizeof(result));
			ss1 = my_csv(ss1,result);
			printf("%d: %s\n",n,result);
			n++;
			if (n == 33)
				break;
		}
		
		i++;
		if (i > 10){
			break;
		}
	}
	fclose(fp);
	return 0;
}
