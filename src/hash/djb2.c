#include "hash/djb2.h"

usize GSHL_hash_djb2(const char *string)
{
    usize hash = 5381;

    for (unsigned char *c = (unsigned char *)string; *c != '\0'; ++c) {
        hash = (hash << 5) + hash + *c; // hash * 33 + c
    }

    return hash;
}
