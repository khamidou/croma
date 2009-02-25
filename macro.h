#ifndef __CROMA_MACRO__
#define __CROMA_MACRO__

/*
  A block is the memory representation of a {{ }} block, along with its 
  parameters.
  A block has a name if it is created using a define() call.

*/

struct croma_block {
	char *name;
	char **parameters;
	char *contents;
	TAILQ_ENTRY(croma_block) blocks;
};

TAILQ_HEAD(croma_blocks_head, croma_block) blocks_head;

/* this function inserts a block at the end of the queue.
   if it fails, it bails out
*/

struct croma_block* alloc_and_insert_block(void);

#endif
