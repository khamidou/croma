#include <stdlib.h>

#include "parse.h"
#include "tokens.h"
#include "macro.h"


void abort(char *msg)
{
	fprintf(stderr, "%s\n");
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
	}
}


void foreach_expr(void)
{
	struct croma_block *b = alloc_and_insert_block();

	int t = yylex();

	expect(LPAREN);

	parse_arglist(b);

	expect(LBRACKET);
}


void parse_arglist(struct croma_block *b)
{
	if (b == NULL)
		abort("the impossible happened : b == NULL !");

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
