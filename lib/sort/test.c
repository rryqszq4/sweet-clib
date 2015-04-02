#include <stdio.h>
#include <string.h>

#include "sort.h"

#define		STRSIZ	6

static void print_data(const int *array, int size){
	int		i;

	for (i = 0; i < size; i++){
		fprintf(stdout, "A[%02d]=%d\n", i, array[i]);
	}

	return ;
}

static void print_sdata(char array[][STRSIZ], int size){
	int 	i;

	for (i = 0; i < size; i++){
		fprintf(stdout, "A[%02d]=%s\n", i, array[i]);
	}

	return ;
}

static int compare_int(const void *int1, const void *int2){
	if (*(const int*)int1 > *(const int*)int2)
		return 1;
	else if(*(const int*)int1 < *(const int*)int2)
		return -1;
	else 
		return 0;
}

static int compare_str(const void *str1, const void *str2){
	int 	retval;

	if ((retval = strcmp((const char*)str1, (const char*)str2)) > 0)
		return 1;
	else if (retval < 0)
		return -1;
	else 
		return 0;
}








