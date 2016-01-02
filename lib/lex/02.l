%{
/*
 * The exemple 02
 *
 */

int
yywrap(void)
{
	return 1;
}
%}
%%

[\t ]+

is |
am |
are |
were |
was |
be |
being |
do |
dose |
did |
will |
would |
should |
can |
could |
has |
have |
had |
go 			{ printf("%s: is a verb\n", yytext); }
[a-zA-Z]+ 	{ printf("%s: is not a verb\n", yytext); }

.|\n 		{ ECHO; }
%%

main()
{
	yylex();
}