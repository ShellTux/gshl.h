#include "hash/fnv1a.h"

u32 GSHL_hash_fnv1a32(const char *const input)
{
    u32 hash = 2166136261u; // FNV offset basis

    for (char *c = (char *)input; *c != '\0'; ++c) {
        hash ^= (unsigned char)(*c);
        hash *= 16777619; // FNV prime
    }

    return hash;
}

#ifdef GSHL_TESTS
#    include "test/mod.h"

GSHL_TEST(hash_fnv1a32)
{
    GSHL_TEST_EQUAL(GSHL_hash_fnv1a32("example"), 0x8BF23EA1);
    GSHL_TEST_EQUAL(GSHL_hash_fnv1a32("test"), 0xAFD071E5);
    GSHL_TEST_EQUAL(GSHL_hash_fnv1a32("hash"), 0xCEC577D1);
}

#endif
