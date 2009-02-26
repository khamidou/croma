/*
  croma, a macro processor.
  
*/

#include <stdio.h>
#include <stdlib.h>

extern FILE *yyin;

int main(int argc, char **argv)
{
	yyin = stdin;
	init_parser();
	yyparse();

}
