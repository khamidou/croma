#include "errors.h"

void fail(char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);

	fprintf(stderr, fmt, ap);

	va_end(ap);

	exit(EXIT_FAILURE);
}

