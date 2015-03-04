#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bns_regex.h"
#include "chtbl.h"
#include "list.h"
#include "bns_hash.h"
#include "bns_util.h"

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
	char *title[400];
	char *ss1;
	char **tp;
	int n,i;
	char *enter = "\n";
	int block = 0;
	char *atk_min;

	char *regex_patten = "<[0-9a-zA-Z]+\\s[0-9a-zA-Z_=\"\\.]+>(.*)</font";
	char *regex_icon = "([0-9]+),[0-9]+,[0-9]+";
	char *regex_loc_k = "path=\"\"[0-9\\.]+([a-zA-Z_]+)\"\"";
	char *regex_loc_v = "<image.*/>(.*)\"{0,1}";
	char *regex_consum = "item:([0-9]+)";


	CHTbl 		htbl;
	SweetListKv *data;

	CHTbl 		set_item_3;
	SweetListKv	*set_item_3_data;
	CHTbl 		set_item_5;
	SweetListKv	*set_item_5_data;
	CHTbl 		set_item_8;
	SweetListKv	*set_item_8_data;
	CHTbl 		effect_equip;
	SweetListKv	*effect_equip_data;

	int tmp_id;
	char *ptr1;
	int loc_i = 0;
	char *ptr2;
	int loc_2 = 0;
	char *ptr3[6];
	int loc_3 = 0;
	int j;
	char *file_icon = "input/icontexture_81.txt";
	char *file_set = "input/set_item_143.txt";
	char *file_effect = "input/effect_equip_43.txt";

	if (argc < 2){
		fprintf(stderr,"error: no file!\n");
		exit(1);
	}

	if (argc < 3){
		fprintf(stderr, "error: no start_gem_id\n");
		exit(1);
	}

	if (argc < 4){
		fprintf(stderr, "error: no end_gem_id\n");
		exit(1);
	}

	if ((fp = fopen(argv[1],"r")) == NULL){
		printf("error!");
		return -1;
	}

	//init
	create_icon_hash(&htbl,file_icon);
	create_setitem_hash(&set_item_3, file_set, 5);
	create_setitem_hash(&set_item_5, file_set, 7);
	create_setitem_hash(&set_item_8, file_set, 10);
	create_setitem_hash(&effect_equip, file_effect, 87);
	print_table(&set_item_3);
	print_table(&set_item_5);
	print_table(&set_item_8);
	print_table(&effect_equip);


	line_num = 1;
	while (!feof(fp)){
		memset(strline, 0, sizeof(strline));
		fgets(strline, LINE_BUF,fp);
		//printf("%s\n\n",strline);
		
		ss1 = (char*)malloc(sizeof(char)*(LINE_BUF+1));
		sprintf(ss1, "%s", strline);
		
		n = 0;
		block = 0;

		/*if (line_num == 1){
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
				if (n == 0 && (atoi(tmp_title) >= atoi(argv[2]) && atoi(tmp_title) <= atoi(argv[3]) )){
				//if (n == 0 && (atoi(tmp_title) >= 623934 && atoi(tmp_title) <= 624058 )){
				//if (n == 0 && (atoi(tmp_title) >= 2607750 && atoi(tmp_title) <= 2607877 )){
					block = 1;
				}
				if (block){
					switch (n){
						case 0:
							printf("{\"_id\":\"%s\",",tmp_title);
							break;
						case 4:
							printf("\"price\":%s,", tmp_title);
							break;
						case 8:
							if (!strcmp(tmp_title,"gam1(7)"))
								printf("\"game_category\":\"1\","); 
							else if (!strcmp(tmp_title,"gan2(8)"))
								printf("\"game_category\":\"2\","); 
							else if (!strcmp(tmp_title,"jin3(9)"))
								printf("\"game_category\":\"3\","); 
							else if (!strcmp(tmp_title,"son4(10)"))
								printf("\"game_category\":\"4\","); 
							else if (!strcmp(tmp_title,"ri5(11)"))
								printf("\"game_category\":\"5\","); 
							else if (!strcmp(tmp_title,"gon6(12)"))
								printf("\"game_category\":\"6\",");
							else if (!strcmp(tmp_title,"tae7(13)"))
								printf("\"game_category\":\"7\","); 
							else if (!strcmp(tmp_title,"geon8(14)"))
								printf("\"game_category\":\"8\","); 
							break;
						case 24:
							printf("\"gear_score\":%s,", tmp_title);
							break;
						case 25:
							printf("\"需要等级\":%s,",tmp_title);
							break;
						case 34:
							if (!strcmp(tmp_title,"7")){
								printf("\"item_grade\":\"6\",");
							}else{
								printf("\"item_grade\":\"%s\",",tmp_title);
							}
							break;
						case 90:
							printf("\"命中\":%s,",tmp_title);
							break;
						case 94:
							printf("\"穿刺\":%s,",tmp_title);
							break;
						case 98:
							printf("\"暴击\":%s,",tmp_title);
							break;
						case 102:
							printf("\"暴击防御\":%s,",tmp_title);
							break;
						case 105:
							printf("\"闪避\":%s,",tmp_title);
							break;
						case 108:
							printf("\"格挡\":%s,",tmp_title);
							break;
						case 123:
							printf("\"最大生命\":%s,",tmp_title);
							break;
						case 134:
							printf("\"max_value\":%s,", tmp_title);
							break;
						case 229:
							printf("\"name\":\"%s\",",tmp_title);
							break;
						case 232:
							if ((data = (SweetListKv *)malloc(sizeof(SweetListKv))) == NULL){
								return 1;
							}
							data->key = (char *)malloc(sizeof(char)*(strlen(filter_regex(regex_icon,tmp_title))+1));
							strcpy(data->key, filter_regex(regex_icon,tmp_title));
							
							//printf("icon:%s,%s,%s\n",filter_regex(regex_icon,tmp_title),data->key,chtbl_find_kv(&htbl, &data)->value);
							printf("\"icon\":\"%s\",",chtbl_find_kv(&htbl, &data)->value);
							free(data);

							break;
						case 263:
							printf("\"title\":\"%s\",",tmp_title);
							break;
						case 266:
							printf("\"location\":[");
							ptr1 = strntok(tmp_title, "<br/>", 5);
							loc_i = 0;
							while(ptr1 != NULL){
								//printf("%s\n", ptr1);
								if (filter_regex(regex_loc_k,ptr1) != NULL){
									//printf("%s\n", ptr1);
									if (loc_i > 0){printf(",");}
									printf("[\"%s\",", str_tolower(filter_regex(regex_loc_k,ptr1)));
									printf("\"%s\"]", str_delchar(filter_regex(regex_loc_v,ptr1),"\""));
									loc_i++;
								}
								//printf("%s\n", __strntok);
								ptr1 = strntok(__strntok, "<br/>", 5);	
							}
							printf("],");
							break;
						case 267:
							printf("\"description\":\"%s\",", tmp_title);
							break;
						case 287:
							ptr2 = strntok(tmp_title,",1,",3);
							loc_2 = 0;
							while (ptr2 != NULL){
								if (filter_regex(regex_consum,ptr2) != NULL){
									ptr3[loc_2] = (char *)malloc(sizeof(char)*strlen((filter_regex(regex_consum,ptr2))+1));
									strcpy(ptr3[loc_2],filter_regex(regex_consum,ptr2));
									loc_2++;
								}
								ptr2 = strntok(__strntok,",1,",3);
							}
							break;
						case 288:
							printf("\"consumable\":[");
							if (loc_2 > 0){
								ptr2 = strntok(tmp_title,",",1);
								loc_2 = 0;
								while (ptr2 != NULL){
									if (ptr3[loc_2] != NULL){
										if (loc_2 > 0){
											printf(",");
										}
										printf("[[\"%s\",\"grocery\"],%s]", ptr3[loc_2],ptr2);
										free(ptr3[loc_2]);
										ptr3[loc_2] = NULL;
									}
									loc_2++;
									ptr2 = strntok(__strntok,",",1);
								}
							}
							printf("],");
							break;
						case 289:
							printf("\"money_cost\":%s", tmp_title);
							printf("}\n");
						default:
							break;
					}	
					//printf("#%d %s: %s\n",n,title[n],tmp_title);
				}
				n++;
			}
		}*/


		free(ss1);
		line_num++;
		/*if (line_num > 2){
			break;
		}*/
	}
	
	for (i = 0; i < 400; i++){
		free(title[i]);
	}

	// free hash table
	remove_icon_hash(&htbl);
	remove_setitem_hash(&set_item_3);
	remove_setitem_hash(&set_item_5);
	remove_setitem_hash(&set_item_8);
	remove_setitem_hash(&effect_equip);

	fclose(fp);
	return(0);
}