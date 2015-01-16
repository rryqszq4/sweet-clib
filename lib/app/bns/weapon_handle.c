#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bns_regex.h"
#include "chtbl.h"
#include "list.h"

#define LINE_BUF 8192*2
#define TITLE_BUF 8192
#define PRIME_TBLSIZ 1024

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


int hashpjw(const void *key)
{
	const char		*ptr;
	unsigned int	val;

	val = 0;
	ptr = key;

	while(*ptr != '\0'){
		unsigned int tmp;
		val = (val << 4) + (*ptr);
		if (tmp = (val & 0xf0000000)) {
			val = val ^ (tmp >> 24);
			val = val ^ tmp;
		}
		ptr ++;
	}

	return val % PRIME_TBLSIZ;
}

static int match_sweet_list_kv(const SweetListKv *data1, const SweetListKv *data2)
{
	return strcmp(data1->key,data2->key) == 0;
}

static void print_table(const CHTbl *htbl)
{
	ListElmt	*element;
	int			i;
	SweetListKv *data;

	fprintf(stdout, "Table size is %d\n", chtbl_size(htbl));
	for (i=0; i < PRIME_TBLSIZ; i++){
		fprintf(stdout, "Bucket[%03d]=", i);
		for (element = list_head(&htbl->table[i]); element != NULL; element = list_next(element)){
			data = list_data(element);
			fprintf(stdout, "[%s,%s] -> ",data->key,data->value);
		}
		fprintf(stdout, "\n");
	}

	return ;
}

int create_icon_hash(CHTbl *htbl){
	FILE *fpi;
	char *ss2;
	char icon_line[1024+1];
	char tmp[1024+1];
	char *enter = "\n";
	int n,line_num;
	SweetListKv *data;

	if ((fpi = fopen("input/utf8_icontexture_79.txt","r")) == NULL){
		fprintf(stderr, "error: no input/utf8_icontexture_79.txt file!\n");
		return 1;
	}

	if (chtbl_init_kv(htbl, PRIME_TBLSIZ, hashpjw, match_sweet_list_kv, free) != 0){
		fprintf(stderr, "error: init hash table fails!\n");
		return 2;
	}

	line_num = 1;
	while (!feof(fpi)){
		memset(icon_line, 0, sizeof(icon_line));
		fgets(icon_line, 1024, fpi);
		ss2 = (char *)malloc(sizeof(char)*(1024+1));
		sprintf(ss2, "%s", icon_line);

		if ((data = (SweetListKv *)malloc(sizeof(SweetListKv))) == NULL)
			return 1;

		n = 0;
		while(*ss2 != *enter){
			memset(tmp, '\0', sizeof(char)*(1024+1));
			sprintf(ss2, "%s", my_csv(ss2, tmp));
			switch (n) {
				case 0:
					//printf("%s, ",tmp);
					data->key = (char *)malloc(sizeof(char)*(strlen(tmp)+1));
					strcpy(data->key,tmp);
					//sprintf(data->key,"%s",tmp);
					break;
				case 2:
					//printf("%s ", tmp);
					data->value = (char *)malloc(sizeof(char)*(strlen(tmp)+1));
					strcpy(data->value, tmp);
					break;
				default:
					break;
			}

			n++;
		}

		//printf("%s,%s\n",data->key,data->value);
		if (chtbl_insert_kv(htbl,data) != 0){
			return 1;
		}

		free(ss2);
		line_num++;
	}

	close(fpi);
	return 0;
}

void remove_icon_hash(CHTbl * htbl){
	chtbl_destroy(htbl);
	return ;
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

	char *regex_patten = "<[0-9a-zA-Z]+\\s[0-9a-zA-Z_=\"\\.]+>(.*)</font>";
	char *regex_icon = "([0-9]+),[0-9]+,[0-9]+";

	CHTbl 		htbl;
	SweetListKv *data;

	if (argc != 2){
		fprintf(stderr,"error: no file!\n");
		exit(1);
	}

	if ((fp = fopen(argv[1],"r")) == NULL){
		printf("error!");
		return -1;
	}

	// init
	create_icon_hash(&htbl);

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
			while (*ss1 != *enter){
				memset(tmp_title, 0 ,sizeof(char)*(TITLE_BUF+1));
				sprintf(ss1, "%s", my_csv(ss1,tmp_title));
				if (n == 0 && (atoi(tmp_title) >= 2607701 && atoi(tmp_title) <= 2607742 ) ){
					block = 1;
				}
				if (block){
					switch (n){
						case 0:
							printf("ID:%s\n",tmp_title);
							printf("weapons:[\"%s\"]\n",tmp_title);
							break;
						case 230:
							printf("名称:%s\n",tmp_title);
							break;
						case 35:
							printf("品级:%s\n",tmp_title);
							break;
						case 8:
							if (!strcmp(tmp_title,"sword(1)"))
								printf("武器类型:2,剑\n");
							else if (!strcmp(tmp_title,"gauntlet(2)"))
								printf("武器类型:3,拳套\n");
							else if (!strcmp(tmp_title,"axe(3)"))
								printf("武器类型:7,斧头\n");
							else if (!strcmp(tmp_title,"staff(4)"))
								printf("武器类型:9,法杖\n");
							else if (!strcmp(tmp_title,"aura-bangle(5)"))
								printf("武器类型:4,彩绫\n");
							else if (!strcmp(tmp_title,"dagger(6)"))
								printf("武器类型:10,短刀\n");
							break;
						case 91:
							printf("命中:%s\n",tmp_title);
							break;
						case 95:
							printf("穿刺:%s\n",tmp_title);
							break;
						case 99:
							printf("暴击:%s\n",tmp_title);
							break;
						case 103:
							printf("暴击防御:%s\n",tmp_title);
							break;
						case 106:
							printf("闪避:%s\n",tmp_title);
							break;
						case 109:
							printf("格挡:%s\n",tmp_title);
							break;
						case 124:
							printf("最大生命:%s\n",tmp_title);
							break;
						case 268:
							printf("攻击效果:%s\n",filter_regex(regex_patten,tmp_title));
							break;
						case 25:
							printf("需要等级:%s\n",tmp_title);
							break;
						case 28:
							if (!strcmp(tmp_title,"1"))
								printf("职业:[\"剑师\"]\n");
							else if (!strcmp(tmp_title,"2"))
								printf("职业:[\"拳师\"]\n");
							else if (!strcmp(tmp_title,"3"))
								printf("职业:[\"气功师\"]\n");
							else if (!strcmp(tmp_title,"5"))
								printf("职业:[\"力士\"]\n");
							else if (!strcmp(tmp_title,"6"))
								printf("职业:[\"召唤师\"]\n");
							else if (!strcmp(tmp_title,"7"))
								printf("职业:[\"刺客\"]\n");
							else if (!strcmp(tmp_title,"8"))
								printf("职业:[\"灵剑士\"]\n");
							break;
						case 13:
							printf("不可交易:%s\n",tmp_title);
							break;
						case 4:
							printf("售价:%s\n",tmp_title);
							break;
						case 24:
							printf("gear_score:%s\n",tmp_title);
							break;
						case 233:
							if ((data = (SweetListKv *)malloc(sizeof(SweetListKv))) == NULL){
								return 1;
							}
							data->key = (char *)malloc(sizeof(char)*(strlen(filter_regex(regex_icon,tmp_title))+1));
							strcpy(data->key, filter_regex(regex_icon,tmp_title));
							
							printf("icon:%s,%s,%s\n",filter_regex(regex_icon,tmp_title),data->key,chtbl_find_kv(&htbl, &data)->value);
							free(data);

							break;
						default:
							break;
					}
					//printf("#%d %s: %s\n",n,title[n],tmp_title);
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
