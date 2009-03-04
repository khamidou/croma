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

#endif
