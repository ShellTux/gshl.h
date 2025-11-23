#include <stdlib.h>
#define GSHL_IMPLEMENTATION
#define GSHL_STRIP_PREFIX
#undef GSHL_TESTS
#include "../gshl.h"

static usize hash_string(const usize table_size,
                         const union GSHL_HashTableKey key)
{
    usize hash = 0;

    for (char *c = key.string; *c != '\0'; ++c) {
        hash = (hash << 5) + *c;
    }

    return hash % table_size;
}

usize hash_isize(const usize table_size, const GSHL_HashTableKey key)
{
    const isize value = key.isize;

    // Use bit manipulation and modulus to ensure it's within the table size
    return (usize)(value ^ (value >> (sizeof(isize) * 8 - 1))) % table_size;
}

static isize keycmp_string(const HashTableKey key1, const HashTableKey key2)
{
    return strcmp(key1.string, key2.string);
}

int main(void)
{
    {
        HashTable ht = {};
        // NOTE: When key type is not a primitive type (ex: a pointer to a null
        // terminated string) we need to provide a function to do key
        // comparison. Otherwise a default comparison function will be used to
        // compare keys
        HashTable_init(&ht, char *, i32, hash_string, .keycmp = keycmp_string);

        HashTable_insert(&ht, .string = "foo", .i32 = 35);
        HashTable_insert(&ht, .string = "bar", .i32 = 34);
        HashTable_insert(&ht, .string = "baz", .i32 = 69);

        // NOTE: 2nd and 4th arguments are printf-like format strings to print
        // key and value respectively. And the 3rd and 5th arguments are the
        // names available in the `union GSHL_HashTableValue`, feel free to
        // check all of it's possible values.
        // It's up to the user to ensure the correct names
        HashTable_print(ht, "\"%s\"", string, "%i", i32);

        const char keys[][10] = {"baz", "asdf", "foo"};
        ARRAY_FOREACH(keys, char *key)
        {
            // NOTE: Here the hash table is passed by pointer to avoid copying
            // it's entire struct, but expect HashTable_search to not modify
            // it's internal state.
            const HashTableValue *value =
                HashTable_search(&ht, .string = (char *)key);
            if (value) {
                printf("ht[\"%s\"] = Some(%i)\n", key, value->i32);
            }
            else {
                printf("ht[\"%s\"] = None\n", key);
            }
        }

        if (!HashTable_destroy(&ht)) {
            return 1;
        }
    }

    srand(0);

    {
        HashTable triple = {};
        HashTable_init(&triple, isize, isize, hash_isize);

        for (usize i = 0; i < 50; ++i) {
            const isize key = rand();
            HashTable_insert(&triple, .isize = key, .isize = key * 3);
        }
        HashTable_insert(&triple, .isize = 23, .isize = 23 * 3);

        HashTable_print(triple, "%li", isize, "%li", isize);

        const isize keys[] = {1059961393, 569, 23, 1914544919};
        ARRAY_FOREACH(keys, isize key)
        {
            const HashTableValue *value =
                HashTable_search(&triple, .isize = key);
            if (value) {
                printf("%li * 3 = %li\n", key, value->isize);
            }
            else {
                printf("%li * 3 = ?\n", key);
            }
        }

        HashTable_destroy(&triple);
    }

    return 0;
}
