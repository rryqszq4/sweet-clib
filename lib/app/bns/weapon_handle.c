#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
		block = 0;

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
				if (n == 0 && strcmp(tmp_title,"2607623") == 0){
					block = 1;
				}
				if (block){
					if (n == 0)
						printf("ID:%s\n",tmp_title);
					if (!strcmp(title[n],"name2"))
						printf("名称:%s\n",tmp_title);
					if (!strcmp(title[n],"item_grade"))
						printf("品级:%s\n",tmp_title);
					if (!strcmp(title[n],"game_category_3")){
						if (!strcmp(tmp_title,"sword(1)"))
							printf("武器类型:2,剑\n");
						if (!strcmp(tmp_title,"gauntlet(2)"))
							printf("武器类型:3,拳套\n");
						if (!strcmp(tmp_title,"axe(3)"))
							printf("武器类型:7,斧头\n");
						if (!strcmp(tmp_title,"staff(4)"))
							printf("武器类型:9,法杖\n");
						if (!strcmp(tmp_title,"aura-bangle(5)"))
							printf("武器类型:4,彩绫\n");
						if (!strcmp(tmp_title,"dagger(6)"))
							printf("武器类型:10,短刀\n");
					}
					if (n == 91)
						printf("命中:%s\n",tmp_title);
					if (n == 95)
						printf("穿刺:%s\n",tmp_title);
					if (n == 99)
						printf("暴击:%s\n",tmp_title);
					if (n == 103)
						printf("暴击防御:%s\n",tmp_title);
					if (n == 106)
						printf("闪避:%s\n",tmp_title);
					if (n == 109)
						printf("格挡:%s\n",tmp_title);
					if (n == 124)
						printf("最大生命:%s\n",tmp_title);
					if (n == 268)
						printf("攻击效果:%s\n",tmp_title);
					if (n == 25)
						printf("需要等级:%s\n",tmp_title);
					if (n == 28)
						printf("职业:\n");
					if (n == 13)
						printf("不可交易:%s\n",tmp_title);
					if (n == 4)
						printf("售价:%s\n",tmp_title);

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

	fclose(fp);
	return(0);
}
