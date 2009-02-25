#include "parse.h"

void abort(char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);

	fprintf(stderr, fmt, ap);

	va_end(ap);

	exit(EXIT_FAILURE);
}

void expect(int t)
{
	int c = yylex();
	if (c != t) {
		fprintf(stderr, "Expected token %s in input, got %s (%s).\n", token_list[t], token_list[c], yytext);
		exit(EXIT_FAILURE);
	}

}

int init_parser(void)
{
	TAILQ_INIT(&blocks_head);
}


int yyparse(void)
{
	int t = yylex();

	switch(t) {

	case FOREACH:
		foreach_expr();
		break;
		
	default:
		abort("garbage somewhere in input");
	}
}


void foreach_expr(void)
{
	struct croma_block *b = alloc_and_insert_block();

	int t = yylex();

	expect(LPAREN);

	parse_arglist(b);

	expect(LBRACKET);

	parse_block(b);

	char *p = b->parameters;
	char *t = b->contents;
	if (t == NULL)
		abort("the impossible happened : b->contents == NULL !, file: %s, line %d", __FILE__, __LINE__);	

	while(strcmp(p, "\0") != 0) {
		while(t != '\0') {
			if (*t == '$' && *t+1 == '$') {
				printf("%s", p);
			t++; /* skip them second '$' */
			} else {
				printf("%c", *t);
			}
			
			t++;
		}
		p++;
	}
}


void parse_arglist(struct croma_block *b)
{
	if (b == NULL)
		abort("the impossible happened : b == NULL !, file: %s, line %d", __FILE__, __LINE__);

	b->parameters = malloc(16*sizeof(char *));
	int i;
	int t = yylex();

	expect(WORD);

	/* FIXME : use realloc to implement infinite length parameter
	   lists
	*/
	for (i = 0; i < 16; i++)
	{
		b->parameters + i = strndup(yytext, MAX_PARAM_LEN);
		expect(COMMA);
		expect(WORD);
	}

	i++;
	b->parameters + i = "\0";

	expect(RPAREN);

}

void parse_block(struct croma_block *b)
{
	int i = 0; /* FIXME : realloc() b->contents on the fly*/
	
	if (b == NULL) 
		abort("the impossible happened : b == NULL !, file: %s, line %d", __FILE__, __LINE__);

	/* Copy the whole buffer in b->contents.
	   FIXME: expand macros before copying contents.
	 */
	int t = yylex();

	while (t != RBRACKET && i < 8192) {
		i += strncat(b->contents, yytext, 8192 - i);
		yylex();
	}

	return;
	
}
