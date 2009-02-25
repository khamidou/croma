#ifndef __TOKENS_H__
#define __TOKENS_H__

enum {
	LPAREN, RPAREN, LBRACKET, RBRACKET, WORD, DOLDOL, SPACES, COMMA, 
	FOREACH,
};

char *token_list[] = { "left parenthesis", 
		       "right parenthesis", 
		       "{{", 
		       "}}", 
		       "word",
		       "$$ sign",
		       "spaces",
		       "comma",
		       "foreach", };

extern int yylex(void);
extern char *yytext;

#endif
