#include <stdlib.h>
#define GSHL_IMPLEMENTATION
#define GSHL_STRIP_PREFIX
#undef GSHL_TESTS
#include "../gshl.h"

static usize hash_djb2_string(const HashTableKey key)
{
    return hash_djb2(key.string);
}

int main(void)
{
    srand(0);

    HashTable ht = {};
    HashTable_init(&ht, char *, usize, hash_djb2_string);

    char bufs[1000][30] = {};
    for (usize i = 0; i < ARRAY_LEN(bufs); ++i) {
        const usize value = rand() % ARRAY_LEN(bufs);
        sprintf(bufs[i], "%lu", value);

        HashTable_insert(&ht, .string = bufs[i], .usize = value);
    }

    for (isize num = RAND_MAX; num >= 0; num -= rand() % 300) {
        const HashTableValue *value =
            HashTable_search(&ht, .string = bufs[num % ARRAY_LEN(bufs)]);
        if (!value) {
            continue;
        }

        printf("value = %lu\n", value->usize);
    }

    HashTable_destroy(&ht);

    return 0;
}
