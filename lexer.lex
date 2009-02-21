%{
/* 
   lexical analyzer for croma
*/

#include "tokens.h"

%}

"(" 		{ return LPAREN; }
")" 		{ return RPAREN; }
"{" 		{ return LBRACKET; }
"}" 		{ return RBRACKET; }
[:alpha:]+ 	{ return WORD; };
"$$" 		{ return DOLDOL; }
[ \t\n]+ 	{ return SPACES; }
