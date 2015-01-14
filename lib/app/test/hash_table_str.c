#include <stdio.h>
#include <string.h>
#include "chtbl.h"
#include "list.h"

#define PRIME_TBLSIZ 32

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

int main(int argc, char **argv)
{
	CHTbl		htbl;
	SweetListKv *data;
	char		*value;
	int			retval,i;

	if (chtbl_init_kv(&htbl, PRIME_TBLSIZ, hashpjw, match_sweet_list_kv, free) != 0)
		return 1;

	if ((data = (SweetListKv *)malloc(sizeof(SweetListKv))) == NULL)
		return 1;
	data->key = "123";
	data->value = "abc";
	if (chtbl_insert_kv(&htbl, data) != 0)
		return 1;
	
	print_table(&htbl);
	
	if ((data = (SweetListKv *)malloc(sizeof(SweetListKv))) == NULL)
		return 1;
	data->key = "456";
	data->value = "def";
	if (chtbl_insert_kv(&htbl,data) != 0)
		return 1;

	print_table(&htbl);

	if (chtbl_remove_kv(&htbl, &data) != 0)
		return 1;
	
	print_table(&htbl);

	chtbl_destroy(&htbl);

	return 0;

}





