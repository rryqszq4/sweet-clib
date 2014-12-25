#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include "mysql.h"

#define SELECT_QUERY "select username from user where userid = %d"

int main(int argc, char **argv){
	MYSQL mysql, *sock;
	MYSQL_RES *res;
	MYSQL_FIELD *fd;
	MYSQL_ROW row;
	char querybuffer[160];

	if (argc != 2){
		fprintf(stderr,"usage: mysql_select <userid>\n\n");
	}

	mysql_init(&mysql);
	if (!(sock = mysql_real_connect(&mysql,"192.168.11.128","root","123456","w2000",0,NULL,0))){
		fprintf(stderr,"Couldn't connect to engine!\n%s\n\n",mysql_error(&mysql));
		perror("");
		exit(1);
	}

	sprintf(querybuffer,SELECT_QUERY,atoi(argv[1]));
	if (mysql_query(sock,querybuffer)){
		fprintf(stderr,"Query fails(%s)\n",mysql_error(sock));
		exit(1);
	}

	if (!(res=mysql_store_result(sock))){
		fprintf(stderr,"Couldn't get result from %s\n",mysql_error(sock));
		exit(1);
	}

	printf("number of fields returned: %d\n",mysql_num_fields(res));

	while(row = mysql_fetch_row(res)){
		printf("There userid #%d 's username is: %s\n",atoi(argv[1]),(((row[0]==NULL)&&(!strlen(row[0])))?"NULL":row[0]));
		puts("query ok !\n");
	}

	mysql_free_result(res);
	mysql_close(sock);
	exit(0);
	return 0;
}
