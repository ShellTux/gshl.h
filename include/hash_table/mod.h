#ifndef INCLUDE_HASH_TABLE_MOD_H_
#define INCLUDE_HASH_TABLE_MOD_H_

#include "types/mod.h"

#ifndef GSHL_HASH_TABLE_SIZE
#    define GSHL_HASH_TABLE_SIZE 256
#endif

/// {{{ Macros

#define GSHL_HashTable_init(HT, KEY_TYPE, VALUE_TYPE, HASH, ...)               \
    do {                                                                       \
        *(HT) = (GSHL_HashTable){__VA_ARGS__};                                 \
        if ((HT)->table_size == 0) {                                           \
            (HT)->table_size = GSHL_HASH_TABLE_SIZE;                           \
        }                                                                      \
        (HT)->table = calloc((HT)->table_size, sizeof(*(HT)->table));          \
        (HT)->key_size = sizeof(KEY_TYPE);                                     \
        (HT)->value_size = sizeof(VALUE_TYPE);                                 \
        (HT)->hash = HASH;                                                     \
        (HT)->description = #KEY_TYPE " -> " #VALUE_TYPE;                      \
        (HT)->key_type = #KEY_TYPE;                                            \
        (HT)->value_type = #VALUE_TYPE;                                        \
    } while (0)

#define GSHL_HashTable_insert(HT, KEY, VALUE)                                  \
    GSHL_HashTable_insert_wrapper(HT, (GSHL_HashTableKey){KEY},                \
                                  (GSHL_HashTableValue){VALUE})

#define GSHL_HashTable_search(HT, KEY)                                         \
    GSHL_HashTable_search_wrapper(HT, (GSHL_HashTableKey){KEY})

#define GSHL_HashTable_delete(HT, KEY)                                         \
    GSHL_HashTable_delete_wrapper(HT, (GSHL_HashTableKey){KEY})

#define GSHL_HashTable_print(HT, KEY_FORMAT, KEY_NAME, VALUE_FORMAT,           \
                             VALUE_NAME)                                       \
    do {                                                                       \
        printf("%s = HashTable [%s][%s] {\n", #HT, (HT).key_type,              \
               (HT).value_type);                                               \
        for (usize GSHL_HashTable_index = 0;                                   \
             GSHL_HashTable_index < GSHL_HASH_TABLE_SIZE;                      \
             GSHL_HashTable_index++) {                                         \
            GSHL_HashTableEntry *current = (HT).table[GSHL_HashTable_index];   \
            if (current == NULL) {                                             \
                continue;                                                      \
            }                                                                  \
            printf("  [" KEY_FORMAT "] = ", current->key.KEY_NAME);            \
            for (; current != NULL; current = current->next) {                 \
                printf("%s" VALUE_FORMAT,                                      \
                       current == (HT).table[GSHL_HashTable_index] ? ""        \
                                                                   : " -> ",   \
                       current->value.VALUE_NAME);                             \
            }                                                                  \
            printf(" (Bucket: %lu)\n", GSHL_HashTable_index);                  \
        }                                                                      \
        printf("}\n");                                                         \
    } while (0)

/// }}}

/// {{{ Types

typedef struct GSHL_HashTableEntry {
    union GSHL_HashTableKey {
        void *opaque; // Generic pointer to key
        char *string;
        u8 u8;
        u16 u16;
        u32 u32;
        u64 u64;
        usize usize;
        i8 i8;
        i16 i16;
        i32 i32;
        i64 i64;
        isize isize;
        bool boolean;
    } key;
    union GSHL_HashTableValue {
        void *opaque; // Generic pointer to value
        char *string;
        u8 u8;
        u16 u16;
        u32 u32;
        u64 u64;
        usize usize;
        i8 i8;
        i16 i16;
        i32 i32;
        i64 i64;
        isize isize;
        bool boolean;
    } value;
    // Pointer to the next entry for collision resolution
    struct GSHL_HashTableEntry *next;
} GSHL_HashTableEntry;

typedef union GSHL_HashTableKey GSHL_HashTableKey;
typedef union GSHL_HashTableValue GSHL_HashTableValue;
typedef usize(GSHL_HashTableHashFunction)(const GSHL_HashTableKey key);

typedef struct GSHL_HashTable {
    // Array of pointers to entries
    GSHL_HashTableEntry **table;
    usize table_size;
    GSHL_HashTableHashFunction *hash;
    char *description;
    char *key_type;
    char *value_type;
    usize key_size;   // Size of the key type
    usize value_size; // Size of the value type
    isize (*keycmp)(const GSHL_HashTableKey key1, const GSHL_HashTableKey key2);
} GSHL_HashTable;

/// }}}

/// {{{ Functions

bool GSHL_HashTable_insert_wrapper(GSHL_HashTable *ht,
                                   const GSHL_HashTableKey key,
                                   const GSHL_HashTableValue value);
GSHL_HashTableValue *
GSHL_HashTable_search_wrapper(const GSHL_HashTable *const ht,
                              const GSHL_HashTableKey key);
bool GSHL_HashTable_delete_wrapper(GSHL_HashTable *ht,
                                   const GSHL_HashTableKey key);
bool GSHL_HashTable_destroy(GSHL_HashTable *ht);

/// }}}

#ifdef GSHL_STRIP_PREFIX
typedef GSHL_HashTable HashTable;
typedef GSHL_HashTableEntry HashTableEntry;
typedef GSHL_HashTableKey HashTableKey;
typedef GSHL_HashTableValue HashTableValue;
#    define HashTable_init GSHL_HashTable_init
#    define HashTable_insert GSHL_HashTable_insert
#    define HashTable_search GSHL_HashTable_search
#    define HashTable_delete GSHL_HashTable_delete
#    define HashTable_destroy GSHL_HashTable_destroy
#    define HashTable_print GSHL_HashTable_print
#endif

#endif // INCLUDE_HASH_TABLE_MOD_H_
