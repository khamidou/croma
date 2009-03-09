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
	TAILQ_INIT(&block->dstrings_head);

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

	free_all_dstrings(b);
	free(b->name);
	TAILQ_REMOVE(&blocks_head, b, blocks);
	free(b);
}

struct croma_block* copy_block(struct croma_block *b)
{
	if (b == NULL)
		return NULL;

	/*
	  FIXME: implement lexical scoping.
	  It's really simple, just make copy_block insert an element in blocks_head.
	 */
	struct croma_block * c = calloc(1, sizeof(struct croma_block));
	
	if (c == NULL)
		fail("Unable to allocate memory block");

	TAILQ_INIT(&b->args_head);
	TAILQ_INIT(&b->dstrings_head);

	if( paste_string(c, b) 	== NULL ||
	    copy_args(c, b) 	== NULL)
		fail("Unable to set up environment for macro expansion.");

	return b;
	
}

struct croma_arg* copy_args(struct croma_block *dest, struct croma_block *src)
{
	if (dest == NULL || src == NULL)
		return NULL;

	struct croma_arg *a1, *a2, *a2_start;

	TAILQ_FOREACH(a2, &src->args_head, params) {
		a1 = alloc_and_insert_string(dest, 0);

		if (a2_start == NULL)
			a2_start = a1;

		a1->name = calloc(strlen(a2->name), sizeof(char));
		memcpy(a1->name, a2->name, strlen(a2->name));
	}

	return a2_start;
	
}

void free_arg(struct croma_block *b, struct croma_arg *a)
{
	if (b == NULL)
		return;

	TAILQ_REMOVE(&b->args_head, a, params);
	free(a->name);
	free(a->value);
	free(a);
}

struct croma_block *lookup_symbol(char *name)
{
	struct croma_block *b;
	TAILQ_FOREACH(b, &blocks_head, blocks) {
		if (strcmp(name, b->name) == 0)
			return b;
	}

	return NULL;
}

struct croma_arg *lookup_arg(struct croma_block *b, char *name)
{
	if (b == NULL)
		return NULL;
	
	struct croma_arg *a;

	TAILQ_FOREACH(a, &b->args_head, params) {
		if (strcmp(name, a->name) == 0)
			return a;
	}

	return NULL;
	
}
