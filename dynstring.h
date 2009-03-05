#ifndef __DYNSTRING_H__
#define __DYNSTRING_H__

#include <stdlib.h>
#include "queue.h"
#include "macro.h"

struct dstring {
	char *p;
	int length;
	TAILQ_ENTRY(dstring) strings;
};

void init_dynstring(struct croma_block *b);
struct dstring* alloc_and_insert_string(struct croma_block *b, size_t len);
struct dstring* alloc_after(struct croma_block *b, struct dstring *before);
void free_dstring(struct croma_block *b, struct dstring *s);
void free_all_dstrings(struct croma_block *b);

struct dstring* break_after(struct dstring *before, char *s);

#endif
