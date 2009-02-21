#include "expansions.h"
#include <stdlib.h>

struct croma_word *alloc_word(struct croma_word *prev, 
			      struct croma_word *next) 
{
	struct croma_word *w = malloc(sizeof(struct croma_word));
	if (w == NULL)
		return NULL;

	w->prev = prev;
	w->next = next;

	if (prev != NULL)
		prev->next = w;

	if (next != NULL)
		next->prev = w;

	return w;
}

void free_words(struct croma_word *w)
{
	if (w->next != NULL)
		free_words(w->next);
	
	if (w->value != NULL)
		free(w->value);

	if (w != NULL)
		free(w);
}

struct croma_word *dup_list(struct croma_word *l)
{
	struct croma_word *w = alloc_word(l, NULL);
	if (w == NULL)
		fatal("Unable to alloc() memory");

	w->strdup(w->value);
	w->next = dup_list(w);

	return w;
}

/* replaces all the occurences of a word in a list */
void replace_occurences(struct croma_word *l, char *o, char *replacement) 
{
	if (l == NULL)
		return;

	if(strcmp(n, l->value) == 0) {
		free(l->value);
		l->value = strdup(replacement);
	}

	if (l->next != NULL)
		replace_occurences(l->next, o, replacement);

}

void process_file(void)
{
	struct 
}

