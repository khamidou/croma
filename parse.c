#include <string.h>
#include "parse.h"

extern char *strndup (__const char *__string, size_t __n);
extern char *yytext;

char *token_list[] = { "left parenthesis", 
		       "right parenthesis", 
		       "{{", 
		       "}}", 
		       "word",
		       "$$ sign",
		       "spaces",
		       "comma",
		       "foreach", NULL};


void expect(int t)
{
	int c = yylex();
	if (c != t) {
		fail("Expected token %s in input, got %s (%s).\n", 
		     token_list[t], token_list[c], yytext);
		fail("unexpected token : %s \n", yytext);
	}

}

void init_parser(void)
{
	TAILQ_INIT(&blocks_head);
}


void yyparse(void)
{
	int t = yylex();

	switch(t) {

	case FOREACH:
		foreach_expr();
		break;
		
	default:
		fail("garbage somewhere in input.\n");
	}
}


void foreach_expr(void)
{
	struct croma_block *b = alloc_and_insert_block();

	expect(LPAREN);

	parse_arglist(b);

	expect(LBRACKET);

	parse_block(b);

	char **p = b->parameters;
	char *c = b->contents;
	if (c == NULL)
		fail("the impossible happened : b->contents == NULL !, file: %s, line %d", __FILE__, __LINE__);	

	while(strcmp(*p, "\0") != 0) {
		while(c != '\0') {
			if (*c == '$' && *c+1 == '$') {
				printf("%s", p);
			c++; /* skip them second '$' */
			} else {
				printf("%c", *c);
			}
			
			c++;
		}
		p++;
	}
}


void parse_arglist(struct croma_block *b)
{
	if (b == NULL)
		fail("the impossible happened : b == NULL !, file: %s, line %d", __FILE__, __LINE__);

	b->parameters = malloc(16*sizeof(char *));
	char **p = b->parameters;

	int i;

	expect(WORD);

	/* FIXME : use realloc to implement infinite length parameter
	   lists
	*/
	for (i = 0; i < 16; i++)
	{
		*p++ = strndup(yytext, MAX_PARAM_LEN);
		expect(COMMA);
		expect(WORD);
	}

	*p++ = "\0";

	expect(RPAREN);

}

void parse_block(struct croma_block *b)
{
	int i = 0; /* FIXME : realloc() b->contents on the fly*/
	
	if (b == NULL) 
		fail("the impossible happened : b == NULL !, file: %s, line %d", __FILE__, __LINE__);

	/* Copy the whole buffer in b->contents.
	   FIXME: expand macros before copying contents.
	 */
	int t = yylex();

	while (t != RBRACKET && i < 8192) {
		strncat(b->contents, yytext, 8192 - i);
		i += strlen(yytext);
		yylex();
	}

	return;
	
}
