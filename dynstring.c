#include "dynstring.h"

struct dstring* alloc_and_insert_string(struct croma_block *b, size_t len)
{
	if (b == NULL)
		return NULL;

	struct dstring *s = calloc(sizeof(struct dstring), 1);
	s->contents = calloc(len, sizeof(char));
	s->length = len;

	if (s == NULL || s->contents == NULL)
		fail("Unable to alloc() memory");

	TAILQ_INSERT_TAIL(&b->dstrings_head, s, strings);

	return s;
}

struct dstring* alloc_after(struct croma_block *b, struct dstring *before)
{
	if (b == NULL || before == NULL)
		return NULL;

	struct dstring *ptr;

	TAILQ_FOREACH(ptr, &b->dstrings_head, strings) {
		if (strncmp(ptr->contents, before->contents, 32) == 0)
		{
			struct dstring *s = calloc(before->length, sizeof(char));
			
			if (s == NULL)
				fail("Unable to alloc() memory");
			
			TAILQ_INSERT_AFTER(&b->dstrings_head, before, s, strings);
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

	if (s->contents != NULL)
		free(s->contents);

	free(s);

	return;
}

void free_all_dstrings(struct croma_block *b)
{
	if (b == NULL)
		return;

	struct dstring *s;
	TAILQ_FOREACH(s, &b->dstrings_head, strings) {
		TAILQ_REMOVE(&b->dstrings_head, s, strings);
		free_dstring(b, s);
	}
}
/*
  break_after breaks a string after *s and allocates it after "before".
 */
struct dstring* break_after(struct croma_block *b, struct dstring *before, char *s)
{
	if (b == NULL || before == NULL)
		return NULL;

	int s_len = before->length - (s - before->contents); /* the length of the string after s */
	
	before->length = s - before->contents;
	
	struct dstring *after =  alloc_after(b, before);
	*s = '\0'; /* terminate the string. FIXME: use realloc() */

	after->contents = calloc(s_len, sizeof(char));
	if (after->contents == NULL)
		fail("Unable to allocate memory ");

	memcpy(after->contents, s, s_len);
	after->length = s_len;

	return after;
}

/*
  This functions copies all the strings in b2 at the end of b1.
 */
struct dstring *paste_string(struct croma_block *b1, struct croma_block *b2)
{
	if (b1 == NULL || b2 == NULL) 
		return NULL;
	
	struct dstring *s1, *s2;
	struct dstring *s2_head = NULL; /* the first element of the pasted string */

	TAILQ_FOREACH(s2, &b2->dstrings_head, strings) {
		s1 = alloc_and_insert_string(b1, 0);

		if (s2_head != NULL)
			s2_head = s1;

		s1->contents = calloc(strlen(s2->contents), sizeof(char));
		memcpy(s1->contents, s2->contents, strlen(s2->contents));
		s1->length = s2->length;
	}

	return s2_head;
}
