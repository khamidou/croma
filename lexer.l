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

%s verbatim

%%

"(" 		{ return LPAREN;	}
")" 		{ return RPAREN;	}
"{{" 		{ BEGIN(verbatim);	return LBRACKET;	}
"}}" 		{ BEGIN(INITIAL);	return RBRACKET;	}
","             { return COMMA;		}
"foreach"       { return FOREACH;	}
"$$" 		{ return DOLDOL;	}

[a-zA-Z0-9]+	{ return WORD;		}
<verbatim>[ \t\n]+ { return SPACES;	}
[ \t\n]+ /* otherwise eat up spaces */ { }
