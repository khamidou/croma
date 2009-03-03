/*
  croma, a macro processor.
  
*/

#include <stdio.h>
#include <stdlib.h>

extern FILE *yyin;

int main(int argc, char **argv)
{
	yyin = stdin;

	if (argc == 2) {
		yyin = fopen(argv[1], "r");
		if (yyin == NULL)
			fail("Unable to open file : %s\n", argv[1]);
	}

	init_parser();
	yyparse();

}
