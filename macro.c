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

	return block;
}
