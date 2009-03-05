#include <string.h>
#include "dynstring.h"
#include "parse.h"

extern char *strndup (__const char *__string, size_t __n);
extern char *yytext;
extern int yylineno;
extern FILE *yyin;

extern int mlexlex();
extern char *mlextext;

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

char *doldol = ""; /* The value assigned to the '$$' variable. */

void expect(int t)
{
	int c = yylex();

	/* Eat up spaces during parsing. */
	while(c == SPACES)
	{
		c = yylex();
	}
	
	if (c != t) {
		fail("Expected token %s in input, got %s (at line %d).\n", 
		     token_list[t], token_list[c], yylineno);
	}

}

void init_parser(void)
{
	TAILQ_INIT(&blocks_head);
	init_dynstring();
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

	parse_arglist(b, yylex, save_foreach_arg);

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
	parse_arglist(b, yylex, save_define_arg);

	parse_block(b);
}

/*
  Instead of creating a using a different function each time we have to parse an argument list,
  we pass instead to parse_arglist() a pointer to the function that will do the job.
 */
void parse_arglist(struct croma_block *b, int (*tokenize)(void), void (*f) (struct croma_block *b))
{
	if (b == NULL)
		fail("the impossible happened : b == NULL !, file: %s, line %d", __FILE__, __LINE__);

	int c = yylex();

	while(c != RPAREN)
	{
		while (c == SPACES) {
			c = tokenize();
		}

		switch(c) {
		case WORD:
			(*f)(b);
			break;

		case RPAREN:
			return;
			break;

		default:
			fail("Unexpected token in argument list");
			break;
		}

		c = tokenize();
	}

}

void save_foreach_arg(struct croma_block *b)
{
	struct croma_arg * arg;

	arg = alloc_and_insert_arg(b);
	arg->value = strdup(yytext);
	if (arg->value == NULL)
		fail("Unable to allocate memory");
}

void save_define_arg(struct croma_block *b)
{
	struct croma_arg * arg;

	arg = alloc_and_insert_arg(b);
	arg->value = strdup(yytext);
	if (arg->value == NULL)
		fail("Unable to allocate memory");
}

void bind_macro_arg(struct croma_block *b)
{
	/*
	  Note : this code assumes that every new element is inserted at the tail.
	 */
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
  Replace arguments replaces all the references to the arguments
  of b in t by their values.
  It occurs only in memory.
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

char *expand_macro(char *s)
{
	if (s == NULL)
		return NULL;

	char *buf = malloc(8192);
	struct croma_block *b;

	int i = 0;

	if (b == NULL)
		fail("Unable to allocate memory");

	int c = mlexlex();

	while(c != -1 && i < 8192) {
		
		switch(c) {

		case WORD:
		char *name = extract_word(mlextext);		
		b = lookup_symbol(name);
		if (b != NULL) {
			
		}
		break;

	} 
	    
	
}
