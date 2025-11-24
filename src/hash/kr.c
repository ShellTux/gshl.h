#include "hash/kr.h"

usize GSHL_hash_kr(const char *const string)
{
    usize h = 0;

    for (unsigned char *c = (unsigned char *)string; *c != '\0'; c++) {
        h += *c + (h << 5) - h;
    }

    return h;
}
