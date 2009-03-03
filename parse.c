#include <string.h>
#include "parse.h"

extern char *strndup (__const char *__string, size_t __n);
extern char *yytext;
extern int yylineno;
extern FILE *yyin;

char *token_list[] = { "left parenthesis", 
		       "right parenthesis", 
		       "{{", 
		       "}}", 
		       "word",
		       "$$ sign",
		       "spaces",
		       "comma",
		       "quote",
		       "foreach", 
		       "define", NULL};

void expect(int t)
{
	int c = yylex();

	/* Eat up spaces during parsing. */
	while(c == SPACES)
	{
		c = yylex();
	}
	
	if (c != t) {
		printf("c %d, t %d, yytext %s\n", c, t, yytext);
/*		fail("Expected token %s in input, got %s (at line %d).\n", 
		     token_list+t, token_list+c, yylineno); */
		fail("EPIC FAIL\n");
	}

}

void init_parser(void)
{
	TAILQ_INIT(&blocks_head);
}


void yyparse(void)
{
	int t;
	while((t = yylex()) != -1) {

		switch(t) {
			
		case FOREACH:
			foreach_expr();
			break;

		case DEFINE:
			define_expr();
			break;

		default:
			printf("%s", yytext);
			continue;
			break;
		}
	}
}


void foreach_expr(void)
{
	struct croma_block *b = alloc_and_insert_block();
	b->name = strdup("Another anonymous foreach block");

	TAILQ_INIT(&b->args_head);
	expect(LPAREN);

	parse_arglist(b);

	expect(LBRACKET);

	parse_block(b);

	struct croma_arg *a;

	if (b->contents == NULL)
		fail("the impossible happened : b->contents == NULL !, file: %s, line %d", __FILE__, __LINE__);	

	TAILQ_FOREACH(a, &b->args_head, params) {
		char *c = b->contents;
		int i = 0;

		for(i = 0; i < b->contents_length && c != NULL; i++) {
			if (*c == '$' && *(c+1) == '$') {
				printf("%s", a->value);
				c++; /* skip the second '$' */
			} else {
				printf("%c", *c);
			}

			c++;
		}

		free_arg(b, a);
	}

	free_block(b);
}

void define_expr(void)
{
	struct croma_block *b = alloc_and_insert_block();

	expect(WORD);
	b->name = strdup(yytext);

	expect(LPAREN);
	parse_arglist(b);
	
}

void parse_arglist(struct croma_block *b)
{
	if (b == NULL)
		fail("the impossible happened : b == NULL !, file: %s, line %d", __FILE__, __LINE__);

	int c = yylex();

	struct croma_arg * arg;

	while(c != RPAREN)
	{
		while (c == SPACES) {
			c = yylex();
		}

		switch(c) {
		case WORD:
			arg = alloc_and_insert_arg(b);
			arg->value = strdup(yytext);
			if (arg->value == NULL)
				fail("Unable to allocate memory");
			break;

		case RPAREN:
			return;
			break;

		default:
			fail("Unexpected token in argument list");
			break;
		}

		c = yylex();
	}

}

void parse_block(struct croma_block *b)
{
	int i = 0; /* FIXME : realloc() b->contents on the fly*/
	
	if (b == NULL) 
		fail("the impossible happened : b == NULL !, file: %s, line %d", __FILE__, __LINE__);

	b->contents = malloc(8192);
	b->contents_length = 0;

	if (b->contents == NULL)
		fail("Unable to alloc memory for input buffer", __FILE__, __LINE__);

	/* Copy the whole buffer in b->contents.
	   FIXME: expand macros before copying contents.
	 */
	int t = yylex();

	while (t != RBRACKET && i < 8192) {
		strncat(b->contents, yytext, 8192 - i);
		i += strlen(yytext);
		t = yylex();
	}

	b->contents_length = i;

	return;
	
}

/*
  This routine extracts a word from a string, without modifying it.
 */
char *extract_word(char *s)
{
	while(*s == ' ' || *s == '\t' || *s == '\n')
		s++;

	char *wend = s;

	while(*wend != ' ' || *wend != '\t' || wend != '\n')
		wend++;

	char *r = malloc((wend - s) * sizeof(char));

	wend = s;

	/* Copy the contents of the word in r. */
	while(*wend != ' ' || *wend != '\t' || wend != '\n')
		*r++ = *wend++;

	return r;
}

/*
  Replace arguments replaces all the references to the arguments
  of b in t by their values.
 */

char *replace_arguments(struct croma_block *b, char *t)
{
	if (b == NULL)
		return;

	char *buf = calloc(8192, sizeof(char));
	char *s = strdup(t);
	char *ss = s;  /* Used only to compute the number of elements copied in buf. */
	char *argname;
	int buflen = s - ss;
	struct croma_arg * arg;

	while(*s != '\0' && buflen < 8192) {

		while(*s == ' ' || *s == '\t' || *s == '\n')
			*buf++ = *s++;
		
		argname = extract_word(s);
		arg = lookup_arg(b, argname);

		if (arg != NULL && arg->value != NULL) {

			strncat(buf, arg->value, 8192 - buflen);

			/* Don't forget to increment the pointers by the number
			   of bytes copied 
			*/

			s += strlen(arg->value);
			buf += strlen(arg->value);

		} else {
			/* If the value is not found, copy the word into the buffer. */
			while(*s != ' ' || *s != '\t' || *s != '\n')
				*buf++ = *s++;		       
		}

		buflen = s - ss;
	}
	
}

char *expand_macro(char *text)
{
	if (text == NULL)
		return NULL;

	char *buf = malloc(8192);
	char *s = strdup(text);
	char *p = strtok(s," \t\n");

	struct croma_block *b;

	int i = 0;

	if (b == NULL)
		fail("Unable to allocate memory");

	
	while(p != NULL) {
		b = lookup_symbol(p);
		/* FIXME : implement argument list parsing */
		p = strtok(NULL, " \t\n");
	}
	    
	
}
