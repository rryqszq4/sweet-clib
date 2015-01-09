#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_BUF 8192*2
#define TITLE_BUF 8192

char *my_csv_2(char *s1, char *s2[],int m){
	char **p;
	p = s2;
	char *enter = "\n";

	while (*s1 != *enter && !isspace(*s1)){
		*p[m] = *s1;
		//printf("123\n");
		p++;
		s1++;
	}

	if (*s1 != *enter && isspace(*s1)){
		s1++;
	}

	return s1;

}

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
	char **tp;
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
				
				title[n] = (char*)malloc(sizeof(char)*strlen(tmp_title)); // 动态分配数组指针
				//title[n] = tmp_title;	// 拷贝指针地址
				strcpy(title[n],tmp_title); // 拷贝指针中的具体内容 
				n++;
			}
		}else {
			while (*ss1 != *enter){
				memset(tmp_title, 0 ,sizeof(char)*(TITLE_BUF+1));
				sprintf(ss1, "%s", my_csv(ss1,tmp_title));
				printf("#%d %s: %s\n",n,title[n],tmp_title);
				n++;
			}
		}


		free(ss1);
		line_num++;
		if (line_num > 2){
			break;
		}
	}
	
	for (i = 0; i < 300; i++){
		free(title[i]);
	}

	fclose(fp);
	return(0);
}
