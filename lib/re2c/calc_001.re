/* re2c calc_001 */

/*!ignore:re2c


- basic interface for string reading

  . We define the macros YYCTYPE, YYCURSOR, YYLIMIT, YYMARKER, YYFILL


*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int scan(char *s, int l)
{
	char *p = s;
	char *q = 0;

#define YYCTYPE		char
#define YYCURSOR	p
#define YYLIMIT		(s + l)
#define YYMARKER	q
#define YYFILL(n)

	for (;;)
	{
/*!re2c
	re2c:indent:top = 2;
	"0"[0-9]+		{	printf("Oct\n"); 	continue;	}
	[1-9][0-9]*		{	printf("Num\n");	continue;	}
	"0"				{	printf("Num\n");	continue;	}
	"+"				{	printf("+\n");		continue;	}
	"-"				{	printf("-\n");		continue;	}
	"\000"		{ printf("EOF\n");	return 0; }
	[^]			{ printf("ERR\n");	return 1; }
*/
	}

}


int main(int argc, char **argv)
{
	if (argc > 1)
	{
		return scan(argv[1], strlen(argv[1]));
	}else 
	{
		fprintf(stderr, "%s <expr>, argv[0]");
		return 1;
	}
}
