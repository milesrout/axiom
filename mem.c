#include <stddef.h>

#include "mem.h"

void *memmove(void *dst, void const *src, size_t count)
{
	size_t i;
	unsigned char *dest = (unsigned char *)dst;
	unsigned char const *source = (unsigned char const *)src;
	if (dest < source) {
		for (i = 0; i < count; i++)
			dest[i] = source[i];
	} else {
		for (i = count; i != 0; i--)
			dest[i-1] =  source[i-1];
	}
	return dst;
}
