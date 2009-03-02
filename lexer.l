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

"(" 		{ return LPAREN;	}
")" 		{ return RPAREN;	}
"{{" 		{ return LBRACKET;	}
"}}" 		{ return RBRACKET;	}
","             { return COMMA;		}
"'"|"\\"		{ return QUOTE;		}
"foreach"       { return FOREACH;	}
"define"	{ return DEFINE;	}
"$$" 		{ return DOLDOL;	}
[ \t\n]+ 	{ return SPACES;	}
[a-zA-Z0-9,?!%*:;$^]+	{ return WORD;		}


