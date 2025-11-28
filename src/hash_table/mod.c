#include "hash_table/mod.h"
#include "macros/mod.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

bool GSHL_HashTable_insert_wrapper(GSHL_HashTable *ht,
                                   const GSHL_HashTableKey key,
                                   const GSHL_HashTableValue value)
{
    const usize index = ht->hash(key) % ht->table_size;

    const GSHL_HashTableEntry def = {.key = key, .value = value, .next = NULL};

    // Collision handling
    if (ht->table[index] == NULL) {
        ht->table[index] = memcpy(malloc(sizeof(def)), &def, sizeof(def));
        GSHL_ASSERT(ht->table[index] != NULL);
        return true;
    }

    GSHL_HashTableEntry *current = NULL;
    for (current = ht->table[index]; current->next != NULL;
         current = current->next) {
        if (ht->keycmp != NULL && ht->keycmp(current->key, key) != 0) {
            continue;
        }
        else if (ht->keycmp == NULL &&
                 memcmp(&current->key, &key, ht->key_size) != 0) {
            continue;
        }

        current->value = value; // Update value if key exists
        return false;
    }

    // Append to the end of the chain
    current->next = memcpy(malloc(sizeof(def)), &def, sizeof(def));
    GSHL_ASSERT(current->next != NULL);

    return true;
}

GSHL_HashTableValue *
GSHL_HashTable_search_wrapper(const GSHL_HashTable *const ht,
                              const GSHL_HashTableKey key)
{
    const usize index = ht->hash(key) % ht->table_size;
    for (GSHL_HashTableEntry *current = ht->table[index]; current != NULL;
         current = current->next) {
        if (ht->keycmp != NULL && ht->keycmp(current->key, key) != 0) {
            continue;
        }
        else if (ht->keycmp == NULL &&
                 memcmp(&current->key, &key, sizeof(key)) != 0) {
            continue;
        }

        return &current->value; // Return pointer to the value
    }

    return NULL; // Not found
}

bool GSHL_HashTable_delete_wrapper(GSHL_HashTable *ht,
                                   const GSHL_HashTableKey key)
{
    const usize index = ht->hash(key) % ht->table_size;
    for (GSHL_HashTableEntry *current = ht->table[index], *prev = NULL;
         current != NULL; prev = current, current = current->next) {
        if (ht->keycmp != NULL && ht->keycmp(current->key, key) != 0) {
            continue;
        }
        else if (ht->keycmp == NULL &&
                 memcmp(&current->key, &key, sizeof(key)) != 0) {
            continue;
        }

        if (prev == NULL) {
            ht->table[index] = current->next; // Remove the head entry
        }
        else {
            prev->next = current->next; // Remove the current entry
        }

        free(current); // Free the entry
        return true;
    }

    return false;
}

bool GSHL_HashTable_destroy(GSHL_HashTable *ht)
{
    for (usize entryI = 0; entryI < ht->table_size; ++entryI) {
        for (GSHL_HashTableEntry *entry = ht->table[entryI], *next = NULL;
             entry != NULL; entry = next) {
            next = entry->next;
            free(entry);
        }
    }

    free(ht->table);

    memset(ht, 0, sizeof(*ht));

    return true;
}

#ifdef GSHL_TESTS
#    include "test/mod.h"

static usize GSHL_test_hash_string(const GSHL_HashTableKey key)
{
    usize hash = 0;

    for (const char *c = key.string; *c != '\0'; ++c) {
        hash = (hash << 5) + *c;
    }

    return hash;
}

static isize keycmp(const GSHL_HashTableKey key1, const GSHL_HashTableKey key2)
{
    return strcmp(key1.string, key2.string);
}

GSHL_TEST(hash_table_string2i32)
{

    GSHL_HashTable ht = {};
    // GSHL_TEST_EQUAL(ht.table, NULL);
    GSHL_TEST_EQUAL(ht.hash, NULL);
    GSHL_TEST_EQUAL(ht.key_size, 0);
    GSHL_TEST_EQUAL(ht.value_size, 0);

    // GSHL_HashTable_init(&ht, hash);
    GSHL_HashTable_init(&ht, char *, i32, GSHL_test_hash_string,
                        .keycmp = keycmp);
    // GSHL_TEST_NEQUAL(ht.table, NULL);
    GSHL_TEST_EQUAL(ht.hash, GSHL_test_hash_string);
    GSHL_TEST_EQUAL(ht.key_size, 8);
    GSHL_TEST_EQUAL(ht.value_size, 4);
    GSHL_TEST_STR_EQUAL(ht.description, "char * -> i32");

    GSHL_TEST_EQUAL(GSHL_HashTable_insert(&ht, .string = "foo", .i32 = 34),
                    true);
    GSHL_TEST_EQUAL(GSHL_HashTable_insert(&ht, .string = "bar", .i32 = 35),
                    true);
    GSHL_TEST_EQUAL(GSHL_HashTable_insert(&ht, .string = "baz", .i32 = 69),
                    true);

    GSHL_TEST_NEQUAL(GSHL_HashTable_search(&ht, .string = "foo"), NULL);
    GSHL_TEST_NEQUAL(GSHL_HashTable_search(&ht, .string = "bar"), NULL);
    GSHL_TEST_NEQUAL(GSHL_HashTable_search(&ht, .string = "baz"), NULL);
    GSHL_TEST_EQUAL(GSHL_HashTable_search(&ht, .string = "asdf"), NULL);
    const char key[] = "foo";
    GSHL_TEST_NEQUAL(GSHL_HashTable_search(&ht, .string = (char *)key), NULL);

    GSHL_TEST_EQUAL(GSHL_HashTable_delete(&ht, .string = "1"), false);
    GSHL_TEST_EQUAL(GSHL_HashTable_delete(&ht, .string = "2"), false);
    GSHL_TEST_EQUAL(GSHL_HashTable_delete(&ht, .string = "3"), false);
    GSHL_TEST_EQUAL(GSHL_HashTable_delete(&ht, .string = "4"), false);
    GSHL_TEST_EQUAL(GSHL_HashTable_delete(&ht, .string = "foo"), true);
    GSHL_TEST_EQUAL(GSHL_HashTable_search(&ht, .string = "foo"), NULL);
    GSHL_TEST_NEQUAL(GSHL_HashTable_search(&ht, .string = "bar"), NULL);

    GSHL_HashTable_destroy(&ht);
}

#endif
