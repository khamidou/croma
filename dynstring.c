#include "dynstring.h"

void init_dynstring(struct croma_block *b)
{	
	if (b == NULL)
		return;

	TAILQ_INIT(&b->dstrings_head);
}

struct dstring* alloc_and_insert_string(struct ccroma_block *b, size_t len)
{
	if (b == NULL)
		return NULL;

	struct dstring *s = calloc(len, sizeof(char));

	if (s == NULL)
		fail("Unable to alloc() memory");

	TAILQ_INSERT_TAIL(&b->dstrings_head, s, strings);

	return block;
}

struct dstring* alloc_after(struct croma_block *b, struct dstring *before)
{
	if (b == NULL || before == NULL)
		return NULL;

	struct dstring *ptr;

	TAILQ_FOREACH(ptr, &b->dstrings_head, strings) {
		if (strncmp(ptr->p, before->p, 32) == 0)
		{
			struct dstring *s = calloc(len, sizeof(char));
			
			if (s == NULL)
				fail("Unable to alloc() memory");
			
			TAILQ_INSERT_AFTER(dstrings_head, before, s, strings);
			break;
		}
	}

	return ptr;
}

void free_dstring(struct croma_block *b, struct dstring *s)
{
	if (b == NULL || s == NULL)
		return;

	TAILQ_REMOVE(&b->dstrings_head, s, strings);

	if (s->p != NULL)
		free(s->p);

	free(s);

	return;
}

void free_all_dstrings(struct croma_block *b)
{
	if (b == NULL)
		return;

	struct dstring *s;
	TAILQ_FOREACH(s, &b->dstrings_head, strings) {
		TAILQ_REMOVE(&dstrings_head, s, strings);
		free_dstring(s);
	}
}
/*
  break_after breaks a string after *s and allocates it after "before".
 */
struct dstring* break_after(struct croma_block *b, struct dstring *before, char *s)
{
	if (b == NULL || before == NULL)
		return NULL;

	int s_len = before->length - (s - p); /* the length of the string after s */
	
	before->length = s - p;
	
	struct dstring *after =  alloc_after(before);
	*s = '\0'; /* terminate the string. FIXME: use realloc() */

	after->p = calloc(s_len, sizeof(char));
	if (after->p == NULL)
		fail("Unable to allocate memory ");

	memcpy(after->p, s, s_len);
	after->length = s_len;

	return after;
}
