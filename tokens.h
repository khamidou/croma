#ifndef __TOKENS_H__
#define __TOKENS_H__

enum {
	LPAREN = 0, RPAREN, LBRACKET, RBRACKET, WORD, DOLDOL, SPACES, COMMA, 
	FOREACH,
};


extern int yylex(void);
extern char *yytext;

#endif
