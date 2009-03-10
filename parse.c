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
	struct dstring *s;

	TAILQ_FOREACH(a, &b->args_head, params) {
		TAILQ_FOREACH(s, &b->dstrings_head, strings) {
			char *c = s->contents;
			int i = 0;
			
			for(i = 0; i < s->length && c != NULL; i++) {
				if (*c == '$' && *(c+1) == '$') {
					printf("%s", a->value);
					c++; 			/* skip the second '$' */
				
				} else {
					printf("%c", *c);
				}
				c++;
			}

			free_arg(b, a);
		}

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


void parse_block(struct croma_block *b)
{
	int i = 0; /* FIXME : realloc() b->contents on the fly*/
	
	if (b == NULL) 
		fail("the impossible happened : b == NULL !, file: %s, line %d", __FILE__, __LINE__);

	struct dstring *s = alloc_and_insert_string(b, 8192);

	struct croma_block *defblock; /* the address of a block defining a macro. */

	s->length = 0;

	int t = yylex();

	while (t != RBRACKET && i < 8192) {
		switch(t) {
		case WORD:
			defblock = lookup_symbol(yytext);
			
			if (defblock != NULL) {
				struct croma_block *env = copy_block(defblock);
				if (env == NULL)
					fail("Unable to allocate memory for macro environment");

				
				expect(LPAREN);

				struct croma_arg *a;
				TAILQ_FOREACH(a, &env->args_head, params) {
					t = yylex();
					if (t == RPAREN)
						break;

					a->value = strdup(yytext);
				}

				expand_macro(env);
			}
		}
		strncat(s->contents, yytext, 8192 - i);
		i += strlen(yytext);
		t = yylex();
	}

	s->length = i;

	return;
}

char *expand_macro(struct croma_block *b)
{
	if (b == NULL)
		return NULL;

	char *buf = malloc(8192);
	char *name;
	struct croma_block *ab; /* In case of a macro called inside another macro. */
	struct croma_arg *a;
	int i = 0;

	if (b == NULL)
		fail("Unable to allocate memory");

	int c = mlexlex();

	while(c != -1 && i < 8192) {
		
		switch(c) {

		case WORD:
			if ((a = lookup_arg(b, mlextext)) != NULL) {
				printf("%s", a->value);
			} else if ((b = lookup_symbol(mlextext)) != NULL) {
				
			} else {
				printf("%s", mlextext);
			}
			break;

		default:
			printf("%s", mlextext);
		}


	} 
	    
	
}
