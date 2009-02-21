#ifndef __EXPANSIONS_H__
#define __EXPANSIONS_H__

struct croma_word {
	char *value;
	struct croma_word *prev, *next;
};

struct croma_word *alloc_word(struct croma_word *prev, 
			      struct croma_word *next);
void free_words(struct croma_word *w);
struct croma_word *dup_list(struct croma_word *l);
void replace_occurences(struct croma_word *l, char *o, char *replacement);

#endif
