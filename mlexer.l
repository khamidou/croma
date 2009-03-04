%{
/*
	mlexer.l - a lexer used to parse the strings in memory.
*/

#undef YY_INPUT

#include "tokens.h"

char *mlexer_ptr;

int mlexwrap()
{
	return(1);
}

void init_mlexer(char *ptr)
{
	mlexer_ptr = ptr;
}

#define YY_INPUT(buf,result,max_size) \
{ \
int c = *mlexer_ptr++; \
if (c == '\0')\
	result = YY_NULL;\
else{\
	result = 1;\
	buf[0] = c;\
	}\
}
%}

%%

[:word:]+			{ return WORD;		}
[:print:]+			{ return OTHER;		}
<<EOF>>				{ return -1;	}

%%