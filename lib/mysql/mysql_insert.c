#include <stdio.h>
#include <stdlib.h>
#include "mysql.h"

void error_quit(const char *str, MYSQL *connection){
	fprintf(stderr, "%s : %d: %s\n",str,mysql_error(connection),mysql_error(connection));
	if (connection != NULL)
		mysql_close(connection);
	exit(1);
}

int main (int argc, char *argv[]){
	MYSQL *my_con = malloc(sizeof(MYSQL));
	MYSQL_RES *my_res;
	MYSQL_FIELD *my_field;
	MYSQL_ROW my_row;
	int row, i;
	int res;

	//连接数据库
	mysql_init(my_con);
	my_con = mysql_real_connect(my_con, "192.168.11.128", "root", "123456", "w2000", 0, NULL ,0);
	if (NULL == my_con)
		error_quit("Connection fail",my_con);
	printf("Connection success\n");

	//向数据库中插入一条纪录
	res = mysql_query(my_con,"insert into user (username) value('test1');");
	if (res != 0)
		error_quit("Insert fail",my_con);
	res = mysql_affected_rows(my_con);
	printf("Inserted %d rows\n",res);

	mysql_free_result(my_res);
	mysql_close(my_con);
	free(my_con);

	return 0;
}
