#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_BUF 8192
#define TITLE_BUF 1024

char *my_csv(char *s1, char *s2){
	char *p;
	p = s2;
	char *enter = "\n";
	
	while (*s1 != *enter && !isspace(*s1)  ){
		*p = *s1;
		p++;
		s1++;
		//break;
	}

	//printf("s1:%s\n", s1);
	if (*s1 != *enter && isspace(*s1)){
		s1++;
	}

	if (*s1 == *enter){
		//printf("123\n");
	}
	
	//printf("s2:%s\n", s2);
	//printf("p:%s\n", p);
	//printf("s1:%s\n", s1);
	return s1;
}

int main(int argc, char *argv[])
{
	FILE *fp;
	int line_num;
	char strline[LINE_BUF+1];
	char tmp_title[TITLE_BUF+1];
	char *title[300];
	char *ss1;
	int n,i;
	char *enter = "\n";

	if (argc != 2){
		fprintf(stderr,"error: no file!\n");
		exit(1);
	}

	if ((fp = fopen(argv[1],"r")) == NULL){
		printf("error!");
		return -1;
	}

	line_num = 1;
	while (!feof(fp)){
		memset(strline, 0, sizeof(strline));
		fgets(strline, LINE_BUF,fp);
		//printf("%s\n\n",strline);
		
		ss1 = (char*)malloc(sizeof(char)*(LINE_BUF+1));
		sprintf(ss1, "%s", strline);
		
		n = 0;

		if (line_num == 1){
			while(*ss1 != *enter){
				memset(tmp_title,0,sizeof(char)*(TITLE_BUF+1));
				sprintf(ss1, "%s", my_csv(ss1,tmp_title));
				
				title[n] = tmp_title;	
				printf("title: %s:\n",tmp_title);
				n++;
				//if (n > 300)
				//	break;
			}
			for (i = 0; i< 300; i++){
				printf("#%d %s\n",i,title[i]);
			}
		}
		
		free(ss1);
		line_num++;
		if (line_num > 1){
			break;
		}
	}

	fclose(fp);
	return(0);
}
