#include "string.h"
#include "types.h"

void *memset(void *dst, int c, unsigned long n) {
    char *cdst = (char *)dst;
    for (unsigned long i = 0; i < n; ++i)
        cdst[i] = c;

    return dst;
}
