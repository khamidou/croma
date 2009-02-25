#ifndef __CROMA_PARSE_H__
#define __CROMA_PARSE_H__

#define MAX_PARAM_LEN 4096

void abort(char *msg);
void init_parser(void);
void yyparse(void);

void foreach_expr(void);
void parse_arglist(struct croma_block *b);

#endif
