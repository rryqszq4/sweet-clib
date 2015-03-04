/**
  *
  * bns 获取单个武器详情
  * gcc -o weapon_info weapon_info.c bns_regex.c chtbl.c list.c bns_hash.c
  *
  **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bns_regex.h"
#include "chtbl.h"
#include "list.h"
#include "bns_hash.h"

#define LINE_BUF 8192*2
#define TITLE_BUF 8192

char *my_csv(char *s1, char *s2){
	char *p;
	p = s2;
	char *enter = "\n";
	char *space = " ";

	while (*s1 != *enter && (*s1 == *space ||  !isspace(*s1))  ){
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
	int block = 0;
	char *atk_min;

	char *regex_patten = "<[0-9a-zA-Z]+\\s[0-9a-zA-Z_=\"\\.]+>(.*)</font>";
	char *regex_icon = "([0-9]+),[0-9]+,[0-9]+";

	CHTbl 		htbl;
	SweetListKv *data;

	int tmp_id;

	char *file_icon = "input/icontexture_81.txt";

	if (argc < 2){
		fprintf(stderr,"error: no file!\n");
		exit(1);
	}

	if (argc < 3){
		fprintf(stderr, "error: no weapon_id\n");
		exit(1);
	}

	if ((fp = fopen(argv[1],"r")) == NULL){
		printf("error!");
		return -1;
	}

	// init
	create_icon_hash(&htbl,file_icon);

	//print_table(&htbl);

	/*if ((data = (SweetListKv *)malloc(sizeof(SweetListKv))) == NULL)
		return 1;
	data->key = "8583";
	data = chtbl_find_kv(&htbl, &data);
	printf("%s\n",data->value);

	free(data);*/
	// free hash table
	//remove_icon_hash(&htbl);

	line_num = 1;
	while (!feof(fp)){
		memset(strline, 0, sizeof(strline));
		fgets(strline, LINE_BUF,fp);
		//printf("%s\n\n",strline);
		
		ss1 = (char*)malloc(sizeof(char)*(LINE_BUF+1));
		sprintf(ss1, "%s", strline);
		
		n = 0;
		block = 0;

		if (line_num == 1){
			while(*ss1 != *enter){
				memset(tmp_title,0,sizeof(char)*(TITLE_BUF+1));
				sprintf(ss1, "%s", my_csv(ss1,tmp_title));
				
				title[n] = (char*)malloc(sizeof(char)*(strlen(tmp_title)+1)); // 动态分配数组指针
				//title[n] = tmp_title;	// 拷贝指针地址
				strcpy(title[n],tmp_title); // 拷贝指针中的具体内容 
				//printf("%s\n",title[n]);
				n++;
			}
		}else {
			atk_min = 0;
			tmp_id = 0;
			while (*ss1 != *enter){
				memset(tmp_title, 0 ,sizeof(char)*(TITLE_BUF+1));
				sprintf(ss1, "%s", my_csv(ss1,tmp_title));
				if (n == 0 && (atoi(tmp_title) == atoi(argv[2])) ){
					block = 1;
				}
				if (block){
					printf("#%d %s: %s\n",n,title[n],tmp_title);
				}
				n++;
			}
		}


		free(ss1);
		line_num++;
		/*if (line_num > 2){
			break;
		}*/
	}
	
	for (i = 0; i < 300; i++){
		free(title[i]);
	}

	// free hash table
	remove_icon_hash(&htbl);

	fclose(fp);
	return(0);
}
