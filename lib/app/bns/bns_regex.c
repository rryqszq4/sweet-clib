#include <stdio.h>
#include <string.h>
#include <regex.h>

#include "bns_regex.h"

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