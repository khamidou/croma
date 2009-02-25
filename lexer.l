%{
/* 
   lexical analyzer for croma
*/

#include "tokens.h"

extern int main(int argc, char **argv);

int yywrap()
{
	return(1);
}

%}

%%

"(" 		{ return LPAREN; }
")" 		{ return RPAREN; }
"{{" 		{ return LBRACKET; }
"}}" 		{ return RBRACKET; }
"foreach"       { return FOREACH; }
[:alpha:]+ 	{ return WORD; };
"$$" 		{ return DOLDOL; }
[ \t\n]+ 	{ return SPACES; }
