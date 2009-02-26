#ifndef __CROMA_PARSE_H__
#define __CROMA_PARSE_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "errors.h"
#include "tokens.h"
#include "macro.h"

#define MAX_PARAM_LEN 4096

void init_parser(void);
void yyparse(void);

void foreach_expr(void);
void parse_arglist(struct croma_block *b);
void parse_block(struct croma_block *b);

#endif
