#include <stddef.h>
#include "string.h"

void *memmove(void *dst, void const *src, size_t count)
{
	unsigned char *dest = (unsigned char *)dst;
	unsigned char const *source = (unsigned char const *)src;
	if (dest < source) {
		for (size_t i = 0; i < count; i++)
			dest[i] = source[i];
	} else {
		for (size_t i = count; i != 0; i--)
			dest[i-1] =  source[i-1];
	}
	return dst;
}
