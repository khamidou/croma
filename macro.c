/*
  macro.c - helper routines for blocks.
 */

#include "macro.h"

struct croma_block* alloc_and_insert_block(void)
{
	struct croma_block *block = malloc(sizeof(struct croma_block));

	if (block == NULL)
		fail("Unable to alloc() memory");

	TAILQ_INSERT_TAIL(&blocks_head, block, blocks);
	TAILQ_INIT(&block->args_head);

	return block;
}

struct croma_arg *alloc_and_insert_arg(struct croma_block *b)
{
	struct croma_arg *a = malloc(sizeof(struct croma_arg));
	
	if (a == NULL)
		fail("Unable to alloc memory");

	TAILQ_INSERT_TAIL(&b->args_head, a, params);

	return a;
}

void free_block(struct croma_block *b)
{
	if (b == NULL)
		return;

	free(b->contents);
	free(b->name);
	TAILQ_REMOVE(&blocks_head, b, blocks);
}
