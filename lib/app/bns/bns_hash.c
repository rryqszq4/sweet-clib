#include "bns_hash.h"

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

int match_sweet_list_kv(const SweetListKv *data1, const SweetListKv *data2)
{
	return strcmp(data1->key,data2->key) == 0;
}

void print_table(const CHTbl *htbl)
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

int create_icon_hash(CHTbl *htbl,char *file_name)
{
	FILE *fpi;
	char *ss2;
	char icon_line[1024+1];
	char tmp[1024+1];
	char *enter = "\n";
	int n,line_num;
	SweetListKv *data;

	if ((fpi = fopen(file_name,"r")) == NULL){
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
					//printf(data->key,"%s",tmp);
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

void remove_icon_hash(CHTbl * htbl)
{
	chtbl_destroy(htbl);
	return ;
}

int create_setitem_hash(CHTbl *htbl,char *file_name, int effect)
{
	FILE *fpi;
	char *ss2;
	char icon_line[8192+1];
	char tmp[8192+1];
	char *enter = "\n";
	int n,line_num;
	SweetListKv *data;

	if ((fpi = fopen(file_name,"r")) == NULL){
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
		fgets(icon_line, 8192, fpi);
		ss2 = (char *)malloc(sizeof(char)*(8192+1));
		sprintf(ss2, "%s", icon_line);

		if ((data = (SweetListKv *)malloc(sizeof(SweetListKv))) == NULL)
			return 1;

		n = 0;
		while(*ss2 != *enter){
			memset(tmp, '\0', sizeof(char)*(1024+1));
			sprintf(ss2, "%s", my_csv(ss2, tmp));
			if (n == 0){
				//printf("%s, ",tmp);
				data->key = (char *)malloc(sizeof(char)*(strlen(tmp)+1));
				strcpy(data->key,tmp);
				//sprintf(data->key,"%s",tmp);
			}
			if (n == effect){
				//printf("%s \n", tmp);
				data->value = (char *)malloc(sizeof(char)*(strlen(tmp)+1));
				strcpy(data->value, tmp);
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

void remove_setitem_hash(CHTbl * htbl)
{
	chtbl_destroy(htbl);
	return ;
}