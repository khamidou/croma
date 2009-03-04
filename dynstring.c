#include "dynstring.h"

void init_dynstring(void)
{	
	TAILQ_INIT(&dstrings_head);
}

struct dstring* alloc_and_insert_string(size_t len)
{
	struct dstring *s = calloc(len, sizeof(char));

	if (s == NULL)
		fail("Unable to alloc() memory");

	TAILQ_INSERT_TAIL(&dstrings_head, s, strings);

	return block;
}

struct dstring* alloc_after(struct dstring *before)
{
	if (before == NULL)
		return NULL;

	struct dstring *ptr;

	TAILQ_FOREACH(ptr, &dstrings_head, strings) {
		if (strncmp(ptr->p, before->p, 32) == 0)
		{
			struct dstring *s = calloc(len, sizeof(char));
			
			if (s == NULL)
				fail("Unable to alloc() memory");
			
			TAILQ_INSERT_AFTER(dstrings_head, dstring, dstring, p)
		}
	}
}
