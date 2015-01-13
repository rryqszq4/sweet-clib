/*
 *# ./regexp_filter '<.*>(.*)<\/.*>' "123 <a> abc </a> 456"
 *
 */

#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <regex.h>


char *filter_regex(char* pattern, char* src){
	regex_t reg;
	regmatch_t pm[10];
	const size_t nmatch = 10;
	int err, i;
	char errbuf[128];
	static char matched[1024];

	err = regcomp(&reg, pattern, REG_EXTENDED);
	if (err != 0){
		regerror(err, &reg, errbuf, sizeof(errbuf));
		fprintf(stderr, "%s: patter '%s'\n", errbuf, pattern);
		return (char*)1;
	}

	err = regexec(&reg, src, nmatch, pm, 0);
	if (err == REG_NOMATCH){
		printf("Sorry, no match ...\n");
		regfree(&reg);
		return (char*)0;
	}else if (err != 0){
		regerror(err, &reg, errbuf, sizeof(errbuf));
		fprintf(stderr, "%s: regcom('%s')\n", errbuf, src);
		return (char*)2;
	}

	for (i = 0; i <= reg.re_nsub; i++){
		memcpy(matched, src + pm[i].rm_so,pm[i].rm_eo - pm[i].rm_so);
		matched[pm[i].rm_eo - pm[i].rm_so] = '\0';
		//printf("match: %s\n", matched);
	}

	regfree(&reg);
	return matched;
}


int main (int argc, char** argv)
{
	char *pat;
	char *str_in;
	char *res;

	pat = argv[1];
	str_in = argv[2];
	res = filter_regex(pat,str_in);
	printf("match: %s\n",res);
}
