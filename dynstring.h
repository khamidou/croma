#ifndef __DYNSTRING_H__
#define __DYNSTRING_H__

#include <stdlib.h>
#include "queue.h"

struct dstring {
	char *p;
	int length;
	TAILQ_ENTRY(dstring) strings;
};

TAILQ_HEAD(dstring_heads, dstring) dstrings_head;

void init_dynstring(void);
struct dstring* alloc_and_insert_string(size_t len);
struct dstring* alloc_after(struct dstring *before);
void free_dstring(struct dstring *s);
void free_all_dstrings(void);

struct dstring* break_after(struct dstring *before, char *s);

#endif
