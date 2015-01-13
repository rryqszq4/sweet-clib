/**
 *
 * gcc -o hash_table hash_table.c ../../_structs/list.c ../../_structs/chtbl.c -I../../_structs
 *
 **/

#include <stdio.h>
#include "chtbl.h"
#include "list.h"

#define	TBLSIZ 11

static int match_char(const void *char1, const void *char2)
{
	return (*(const char *)char1 == *(const char *)char2);
}

static int h_char(const void *key){
	return *(const char *)key % TBLSIZ;
}

static void print_table(const CHTbl *htbl)
{
	ListElmt	*element;
	int			i;

	fprintf(stdout, "Table size if %d\n", chtbl_size(htbl));
	for (i=0; i < TBLSIZ; i++){
		fprintf(stdout, "Bucket[%03d]=", i);
		for (element = list_head(&htbl->table[i]); element != NULL; element = list_next(element)){
			fprintf(stdout, "%c",*(char *)list_data(element));
		}
		fprintf(stdout, "\n");
	}

	return ;
}

int main(int argc, char **argv)
{
	CHTbl	htbl;
	char	*data,
			c;
	int		retval,
			i;

	if (chtbl_init(&htbl, TBLSIZ, h_char, match_char, free) != 0)
		return 1;
	
	for (i = 0; i < TBLSIZ; i++){
		if ((data = (char *)malloc(sizeof(char))) == NULL)
			return 1;
		*data = ((5 + (i * 6)) % 23) + 'A';
		printf("%s\n", data);
		if (chtbl_insert(&htbl, data) != 0)
			return 1;

		print_table(&htbl);
	}


	chtbl_destroy(&htbl);

	return 0;
}








