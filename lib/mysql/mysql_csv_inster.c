#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mysql.h"

char *my_csv(char *s1, char *s2){
	char *p;
	char *comma = ",";
	char *enter = "\n";

	p = s2;
	//printf("isspace:%d\n",isspace(*s1));
	while(isspace(*s1)){
		s1++;
	}
	while(*s1 != *comma && *s1 != *enter){
		*p = *s1;
		p++;
		s1++;
	}
	if (*s1 == *comma){
		s1++;
	}
	//printf("s2:%s\n",s2);
	//printf("%s\n",p);
	//printf("s1:%s\n",s1);
	return s1;
}

void error_quit(const char *str, MYSQL *connection){
	fprintf(stderr, "%s : %d: %s\n",str,mysql_error(connection),mysql_error(connection));
	if (connection != NULL)
		mysql_close(connection);
	exit(1);
}

int main(int argc, char *argv[]){
	FILE *fp;
	char strline[1024];
	char *ss1;
	char result[512];
	char *comma = ",";
	int i,n;
	char temp[128];
	char sql[512];

	MYSQL *my_con = malloc(sizeof(MYSQL));
	MYSQL_RES *my_res;
	MYSQL_FIELD *my_field;
	MYSQL_ROW my_row;
	int row;
	int res;



	if (argc != 3){
		fprintf(stderr,"err: no file!\n");
		exit(1);
	}

	if ((fp = fopen(argv[1],"r")) == NULL){
		printf("error!");
		return -1;
	}

	//连接数据库
	mysql_init(my_con);
	my_con = mysql_real_connect(my_con,"192.168.11.128","root","123456","w2000",0,NULL,0);
	if (NULL == my_con)
		error_quit("Connection fail",my_con);
	printf("Connection success\n");

	if (!mysql_set_character_set(my_con,"utf8")){
		printf("New client character set: %s\n",mysql_character_set_name(my_con));
	}

	i = 0;
	while(!feof(fp)){
		memset(strline,0,sizeof(strline));
		fgets(strline,1024,fp);
		if (i == 0){
			i++;
			continue;
		}
		//printf("%s\n",strline);
		
		memset(sql,0,sizeof(sql));
		memset(temp,0,sizeof(temp));
		strcat(sql,"insert into user (username,ctftype,identity,sex,birthday,address,zip,phone,email,createtime) value (");

		n = 0;
		//ss1 = strline;
		ss1 = (char*)malloc(sizeof(char)*1024);
		sprintf(ss1,"%s",strline);
		if (i > atoi(argv[2])){
			//printf("%s",ss1);
		while(ss1 != NULL){
			memset(result,0,sizeof(result));
			sprintf(ss1,"%s",my_csv(ss1,result));
			//printf("%d: %s",n,ss1);
			//printf("%d: %s\n",n,result);
			if (n == 0){
				sprintf(temp,"'%s',",result);
				strcat(sql,temp);
			}
			if (n == 3){
				sprintf(temp,"'%s',",result);
				strcat(sql,temp);
			}
			if (n == 4){
				sprintf(temp,"'%s',",result);
				strcat(sql,temp);
			}
			if (n == 5){
				sprintf(temp,"'%s',",result);
				strcat(sql,temp);
			}
			if (n == 6){
				sprintf(temp,"'%s',",result);
				strcat(sql,temp);
			}
			if (n == 7){
				sprintf(temp,"'%s',",result);
				strcat(sql,temp);
			}
			if (n == 8){
				sprintf(temp,"'%s',",result);
				strcat(sql,temp);
			}
			if (n == 19){
				sprintf(temp,"'%s',",result);
				strcat(sql,temp);
			}
			if (n == 22){
				sprintf(temp,"'%s',",result);
				strcat(sql,temp);
			}
			if (n == 31){
				sprintf(temp, "'%s');",result);
				strcat(sql,temp);
			}
			
			n++;
			if (n == 32)
				break;
		}
		
			//printf("%d: %s\n",i,sql);
		
		//行数据库中插入一条记录
			res = mysql_query(my_con,sql);
			if (res != 0){
				//error_quit("Insert fail",my_con);
				printf("error\n");
			}else {
				res = mysql_affected_rows(my_con);
				printf("%d,",i);
			}
			//printf("%d,",i);
		}
		free(ss1);
		i++;
		/*if (i > 290044){
			break;
		}*/
	}


	fclose(fp);

	//mysql_free_result(my_res);
	mysql_close(my_con);
	free(my_con);
	
	return 0;
}
