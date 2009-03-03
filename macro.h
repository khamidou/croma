#ifndef __CROMA_MACRO__
#define __CROMA_MACRO__

#include <stdlib.h>
#include "errors.h"
#include "queue.h"

/*
  A block is the memory representation of a {{ }} block, along with its 
  parameters.
  A block has a name if it is created using a define() call.

*/

#include "queue.h"

struct croma_arg {
	char *name;
	char *value;
	TAILQ_ENTRY(croma_arg) params;
};

struct croma_block {
	char *name;
//	char **parameters;
	char *contents;
	int contents_length;
	TAILQ_HEAD(croma_args_head, croma_arg) args_head;
	TAILQ_ENTRY(croma_block) blocks;
};

/*
  blocks_head is our symbol table.
*/

TAILQ_HEAD(croma_blocks_head, croma_block) blocks_head;

/* this function inserts a block at the end of the queue.
   if it fails, it bails out
*/

struct croma_block* alloc_and_insert_block(void);
struct croma_arg *alloc_and_insert_arg(struct croma_block *b);
void free_block(struct croma_block *b);
void free_arg(struct croma_block *b, struct croma_arg *a);
struct croma_block *lookup_symbol(char *name);
struct croma_arg *lookup_arg(struct croma_block *b, char *name);

#endif
