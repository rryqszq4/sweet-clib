#ifndef BNS_UTIL_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char *__strntok;

char* strntok(char *s1, char *s2, int n);

char* str_tolower(char *s1);

char* str_delchar(char *s1, char *s2);
#endif
