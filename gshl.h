#ifndef INCLUDE_GSHL_H_
#define INCLUDE_GSHL_H_
// gshl-priority: 99
// #ifndef INCLUDE_MACROS_MOD_H_
// #define INCLUDE_MACROS_MOD_H_

#include <stdio.h> // IWYU pragma: keep

#ifndef GSHLDEF
#    define GSHLDEF
#endif

#define GSHL_UNUSED(X) (void)(X)
#define GSHL_EXIT(...)                                                         \
    do {                                                                       \
        fprintf(stderr, "%s:%d: ", __FILE__, __LINE__);                        \
        fprintf(stderr, __VA_ARGS__);                                          \
        fprintf(stderr, "\n");                                                 \
        exit(1);                                                               \
    } while (0)
#define GSHL_TODO(...) GSHL_EXIT("TODO: " __VA_ARGS__)
#define GSHL_UNREACHABLE(...) GSHL_EXIT("UNREACHABLE: " __VA_ARGS__)
#define GSHL_LOOP for (;;)
#ifdef GSHL_DEBUG
#    define GSHL_ASSERT(...) assert(__VA_ARGS__)
#else
#    define GSHL_ASSERT(...) GSHL_UNUSED((__VA_ARGS__))
#endif
#define GSHL_unlikely(CONDITION) __glibc_unlikely(CONDITION)

#ifdef GSHL_STRIP_PREFIX
#    define ASSERT GSHL_ASSERT
#    define loop GSHL_LOOP
#    define LOOP GSHL_LOOP
#    define TODO GSHL_TODO
#    define UNREACHABLE GSHL_UNREACHABLE
#    define UNUSED GSHL_UNUSED
#    define unlikely GSHL_unlikely
#endif

// #endif // INCLUDE_MACROS_MOD_H_
// gshl-priority: 90
// #ifndef INCLUDE_TYPES_MOD_H_
// #define INCLUDE_TYPES_MOD_H_

#include <stdbool.h>
#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef uintmax_t usize;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef intmax_t isize;

typedef float f32;
typedef double f64;

typedef char *cstring;

typedef void *pointer;

typedef u32 hex32;
typedef u64 hex64;

typedef int Fd;

// #endif // INCLUDE_TYPES_MOD_H_
// gshl-priority: 85
// #ifndef INCLUDE_STRING_MOD_H_
// #define INCLUDE_STRING_MOD_H_

// #include "types/mod.h"

/// {{{ Macros

#define GSHL_STACK_STRING_LEN(ARRAY) (sizeof(ARRAY) - 1)
#define GSHL_STRING_FOREACH(STRING, VARNAME)                                   \
    for (usize keep = 1, index = 0, exit = false; keep && !exit;               \
         keep = !keep, index++)                                                \
        for (const char VARNAME = (STRING)[index],                             \
                        *VARNAME##P = &(STRING)[index];                        \
             keep && !(exit = VARNAME == '\0'); keep = !keep && *VARNAME##P)

/// }}}

/// {{{ Types

typedef struct GSHL_StringView {
    char *start;
    usize len;
} GSHL_StringView;

/// }}}

/// {{{ Functions

char *GSHL_string_dup(const char *const source);
void GSHL_string_reverse(char *start, char *end);

/// }}}

#ifdef GSHL_STRIP_PREFIX
#    define STACK_STRING_LEN GSHL_STACK_STRING_LEN
#    define StringView GSHL_StringView
#    define string_dup GSHL_string_dup
#    define string_reverse GSHL_string_reverse
#    define STRING_FOREACH GSHL_STRING_FOREACH
#endif

// #endif // INCLUDE_STRING_MOD_H_
// gshl-priority: 85
// #ifndef INCLUDE_HASH_TABLE_MOD_H_
// #define INCLUDE_HASH_TABLE_MOD_H_

// #include "types/mod.h"

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
        GSHL_ASSERT(sizeof(KEY_TYPE) <= sizeof((HT)->table[0]->key));          \
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
                             VALUE_NAME, ...)                                  \
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
                printf(" " __VA_ARGS__);                                       \
            }                                                                  \
            printf(" (Bucket: %lu)\n", GSHL_HashTable_index);                  \
        }                                                                      \
        printf("}\n");                                                         \
    } while (0)

/// }}}

/// {{{ Types

typedef struct GSHL_HashTableEntry {
    union GSHL_HashTableKey {
        const void *opaque; // Generic pointer to key
        const char *string;
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
        const char *string;
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

// #endif // INCLUDE_HASH_TABLE_MOD_H_
// gshl-priority: 85
// #ifndef INCLUDE_DYNAMIC_ARRAY_MOD_H_
// #define INCLUDE_DYNAMIC_ARRAY_MOD_H_

/// {{{ Macros

#ifndef GSHL_DARRAY_INIT_CAPACITY
#    define GSHL_DARRAY_INIT_CAPACITY 256
#else
#    if GSHL_DARRAY_INIT_CAPACITY <= 0
#        error "GSHL_DARRAY_INIT_CAPACITY must be > 0"
#    endif
#endif

#define GSHL_DArrayTypeDecl(NAME, TYPE)                                        \
    typedef struct NAME {                                                      \
        union {                                                                \
            usize size;                                                        \
            usize count;                                                       \
            usize len;                                                         \
            usize length;                                                      \
        };                                                                     \
        usize capacity;                                                        \
        TYPE *items;                                                           \
    } NAME

#define GSHL_DArray_init(DARRAY) (DARRAY)->size = 0

#define GSHL_DArray_append(DARRAY, ITEM)                                       \
    do {                                                                       \
        if (GSHL_unlikely((DARRAY)->items == NULL)) {                          \
            (DARRAY)->capacity = GSHL_DARRAY_INIT_CAPACITY;                    \
            (DARRAY)->items =                                                  \
                calloc((DARRAY)->capacity, sizeof(*(DARRAY)->items));          \
            GSHL_ASSERT((DARRAY)->items != NULL);                              \
        }                                                                      \
                                                                               \
        if (GSHL_unlikely((DARRAY)->count + 1 > (DARRAY)->capacity)) {         \
            (DARRAY)->capacity *= 2;                                           \
            (DARRAY)->items =                                                  \
                realloc((DARRAY)->items,                                       \
                        (DARRAY)->capacity * sizeof(*(DARRAY)->items));        \
            assert((DARRAY)->items != NULL);                                   \
        }                                                                      \
                                                                               \
        (DARRAY)->items[(DARRAY)->count++] = ITEM;                             \
    } while (0)

#define GSHL_DArray_extend(DARRAY, ITEMS, ...)                                 \
    GSHL_DArray_extendn(DARRAY, ITEMS,                                         \
                        sizeof(ITEMS) / sizeof((DARRAY)->items[0]))

#define GSHL_DArray_extendn(DARRAY, ITEMS, ITEMS_N)                            \
    do {                                                                       \
        if (GSHL_unlikely((DARRAY)->items == NULL)) {                          \
            (DARRAY)->capacity = GSHL_DARRAY_INIT_CAPACITY;                    \
            (DARRAY)->items =                                                  \
                calloc((DARRAY)->capacity, sizeof(*(DARRAY)->items));          \
            GSHL_ASSERT((DARRAY)->items != NULL);                              \
        }                                                                      \
                                                                               \
        if (GSHL_unlikely((DARRAY)->count + (ITEMS_N) > (DARRAY)->capacity)) { \
            while ((DARRAY)->count + (ITEMS_N) > (DARRAY)->capacity) {         \
                (DARRAY)->capacity *= 2;                                       \
            }                                                                  \
            (DARRAY)->items =                                                  \
                realloc((DARRAY)->items,                                       \
                        (DARRAY)->capacity * sizeof(*(DARRAY)->items));        \
            assert((DARRAY)->items != NULL);                                   \
        }                                                                      \
                                                                               \
        memcpy(&(DARRAY)->items[(DARRAY)->count], (ITEMS),                     \
               (ITEMS_N) * sizeof((ITEMS)[0]));                                \
        (DARRAY)->count += ITEMS_N;                                            \
    } while (0)

#define GSHL_DArray_insert(DARRAY, INDEX, ITEM)                                \
    do {                                                                       \
        assert(0 <= INDEX && INDEX <= (DARRAY)->count);                        \
        if (GSHL_unlikely((DARRAY)->count >= (DARRAY)->capacity)) {            \
            (DARRAY)->capacity *= 2;                                           \
            (DARRAY)->items =                                                  \
                realloc((DARRAY)->items,                                       \
                        (DARRAY)->capacity * sizeof(*(DARRAY)->items));        \
            assert((DARRAY)->items != NULL);                                   \
        }                                                                      \
        memmove(&(DARRAY)->items[INDEX + 1], &(DARRAY)->items[INDEX],          \
                ((DARRAY)->count - INDEX) * sizeof(*(DARRAY)->items));         \
        (DARRAY)->items[INDEX] = ITEM;                                         \
        (DARRAY)->count++;                                                     \
    } while (0)

#define GSHL_DArray_remove(DARRAY, INDEX)                                      \
    do {                                                                       \
        assert(INDEX < (DARRAY)->count);                                       \
        memmove(&(DARRAY)->items[INDEX], &(DARRAY)->items[INDEX + 1],          \
                ((DARRAY)->count - INDEX - 1) * sizeof(*(DARRAY)->items));     \
        (DARRAY)->count--;                                                     \
    } while (0)

#define GSHL_DArray_destroy(DARRAY)                                            \
    do {                                                                       \
        free((DARRAY)->items);                                                 \
        memset(DARRAY, 0, sizeof(*DARRAY));                                    \
    } while (0)

/// }}}

#ifdef GSHL_STRIP_PREFIX
#    define DArrayTypeDecl GSHL_DArrayTypeDecl
#    define DArray_init GSHL_DArray_init
#    define DArray_append GSHL_DArray_append
#    define DArray_insert GSHL_DArray_insert
#    define DArray_remove GSHL_DArray_remove
#    define DArray_destroy GSHL_DArray_destroy
#endif

// #endif // INCLUDE_DYNAMIC_ARRAY_MOD_H_
// gshl-priority: 80
// #ifndef INCLUDE_FORMAT_MOD_H_
// #define INCLUDE_FORMAT_MOD_H_

// #include "dynamic_array/mod.h"
// #include "string/mod.h"

#include <stdarg.h>
#include <stddef.h>

/// {{{ Macros

#define GSHL_FORMAT_SPECIFIER_MAX_LEN 30
#define GSHL_FORMAT_SPEIFIERS_MAX_PER_TYPE 10

#define GSHL_FORMAT_SPECIFIERS                                                 \
    /* FS(BIT_SHIFT, ENUM, NAME, TYPE, OPTS, ...) */                           \
    FS(0, CSTRING, cstring, cstring, {}, "%s", "string", "cstring")            \
    FS(1, U8, u8, u8, {})                                                      \
    FS(2, U16, u16, u16, {})                                                   \
    FS(3, U32, u32, u32, {}, "%u", "u32")                                      \
    FS(4, U64, u64, u64, {}, "%lu", "u64")                                     \
    FS(5, USIZE, usize, usize, {}, "%lu", "usize")                             \
    FS(6, I8, i8, i8, {})                                                      \
    FS(7, I16, i16, i16, {})                                                   \
    FS(8, I32, i32, i32, {}, "%i", "%d", "i32")                                \
    FS(9, I64, i64, i64, {}, "%li", "%ld", "i64")                              \
    FS(10, ISIZE, isize, isize, {}, "%li", "%ld", "isize")                     \
    FS(11, CHAR, character, char, {}, "%c", "char", "character")               \
    FS(12, POINTER, pointer, pointer, { bool uppercase; }, "%p", "pointer")    \
    FS(13, HEX32, hex32, hex32, { bool uppercase; }, "%x", "%X", "hex32")      \
    FS(14, HEX64, hex64, hex64, { bool uppercase; }, "%lx", "%lX", "hex64")    \
    FS(15, BOOL, boolean, bool, {}, "%B", "boolean", "bool")

/// }}}

/// {{{ Types

GSHL_DArrayTypeDecl(GSHL_FormatString, char);

typedef enum GSHL_FormatSpecifierKind {
    GSHL_FORMAT_SPECIFIER_NONE = 0,
#define FS(BIT_SHIFT, ENUM, NAME, TYPE, OPTS, ...)                             \
    GSHL_FORMAT_SPECIFIER_##ENUM = 1 << BIT_SHIFT,
    GSHL_FORMAT_SPECIFIERS
#undef FS
} GSHL_FormatSpecifierKind;

typedef union GSHL_FormatSpecifierOpts {
#define FS(BIT_SHIFT, ENUM, NAME, TYPE, OPTS, ...)                             \
    struct GSHL_FormatSpecifierOpts_##TYPE OPTS NAME;
    GSHL_FORMAT_SPECIFIERS
#undef FS
} GSHL_FormatSpecifierOpts;

typedef union GSHL_FormatSpecifierValue {
#define FS(BIT_SHIFT, ENUM, NAME, TYPE, OPTS, ...) TYPE NAME;
    GSHL_FORMAT_SPECIFIERS
#undef FS
} GSHL_FormatSpecifierValue;

typedef struct GSHL_FormatSpecifier GSHL_FormatSpecifier;

typedef usize(GSHL_FormatSpecifierWriteFunction)(GSHL_FormatString *,
                                                 const GSHL_FormatSpecifier *);

struct GSHL_FormatSpecifier {
    GSHL_FormatSpecifierKind kind;
    GSHL_FormatSpecifierWriteFunction *write;
    GSHL_FormatSpecifierValue value;
    GSHL_FormatSpecifierOpts opts;
    usize va_size;
    char specifiers[GSHL_FORMAT_SPEIFIERS_MAX_PER_TYPE]
                   [GSHL_FORMAT_SPECIFIER_MAX_LEN];
};

GSHL_DArrayTypeDecl(GSHL_FormatSpecifiers, GSHL_FormatSpecifier);

/// }}}

/// {{{ Functions

GSHL_StringView GSHL_format_wrapper(const char *const restrict format, ...);
GSHL_StringView GSHL_format_wrapperv(const char *const restrict format,
                                     va_list args);

usize GSHL_format_write(GSHL_FormatString *string,
                        const char *const restrict format, ...);
usize GSHL_format_writev(GSHL_FormatString *string,
                         const char *const restrict format, va_list args);

char *GSHL_format(const char *const restrict format, ...);
char *GSHL_formatv(const char *const restrict format, va_list args);

void GSHL_print_registered_format_specifiers(void);

bool GSHL_format_specifier_register(const GSHL_FormatSpecifier fs);

/// }}}

#ifdef GSHL_STRIP_PREFIX
#    define format GSHL_format
#    define formatv GSHL_formatv
#    define print_registered_format_specifiers                                 \
        GSHL_print_registered_format_specifiers
#    define format_write GSHL_format_write
#    define format_writev GSHL_format_writev
#    define FormatSpecifier GSHL_FormatSpecifier
#endif

// #endif // INCLUDE_FORMAT_MOD_H_
// #ifndef INCLUDE_TYPES_LIMITS_H_
// #define INCLUDE_TYPES_LIMITS_H_

/// {{{ Macros

#define U8_MAX ((u8)((1U << 8) - 1))
#define U8_MIN 0

#define U16_MAX ((u16)((1U << 16) - 1))
#define U16_MIN 0

#define U32_MAX ((u32)((1UL << 32) - 1))
#define U32_MIN 0

#define U64_MAX ((u64)((1ULL << 64) - 1))
#define U64_MIN 0

#define USIZE_MAX ((usize)((1U << (sizeof(usize) * 8)) - 1))
#define USIZE_MIN 0

#define I8_MAX ((i8)((1 << 7) - 1))
#define I8_MIN ((i8)(-1 * (1 << 7)))

#define I16_MAX ((i16)((1 << 15) - 1))
#define I16_MIN ((i16)(-1 * (1 << 15)))

#define I32_MAX ((i32)((1U << 31) - 1))
#define I32_MIN ((i32)(-1 * (1U << 31)))

#define I64_MAX ((i64)((1ULL << 63) - 1))
#define I64_MIN ((i64)(-1 * (1ULL << 63)))

#define ISIZE_MAX ((isize)((1 << (sizeof(isize) * 8 - 1)) - 1))
#define ISIZE_MIN ((isize)(-1 * (1 << (sizeof(isize) * 8 - 1))))

#define F32_MAX 340282346638528859811704183484516925440.0000000000000000f
#define F32_MIN -340282346638528859811704183484516925440.0000000000000000f

#define F64_MAX                                                                \
    179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.0000000000000000
#define F64_MIN                                                                \
    -179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.0000000000000000

/// }}}

// #endif // INCLUDE_TYPES_LIMITS_H_
// vim: foldmethod=marker
// #ifndef INCLUDE_TEST_MOD_H_
// #define INCLUDE_TEST_MOD_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>  // IWYU pragma: keep
#include <string.h> // IWYU pragma: keep

// #include "ansi/colors.h"
// #include "macros/mod.h"
// #include "types/mod.h"

/// {{{ Macros

#define GSHL_TEST(FUNC)                                                        \
    GSHL_TestFunction test_##FUNC;                                             \
    __attribute__((constructor)) void register_test_##FUNC(void)               \
    {                                                                          \
        GSHL_register_test_wrapper(#FUNC, test_##FUNC);                        \
    }                                                                          \
    void test_##FUNC(usize *failed_tests)

#define GSHL_REGISTER_TEST(TEST_FUNC)                                          \
    GSHL_register_test_wrapper(#TEST_FUNC, TEST_FUNC)

#define GSHL_TEST_EQUAL(EXPR1, EXPR2, ...)                                     \
    do {                                                                       \
        GSHL_TestEqualOpt opt = (GSHL_TestEqualOpt){__VA_ARGS__};              \
                                                                               \
        if ((EXPR1) != (EXPR2)) {                                              \
            *failed_tests += 1;                                                \
            fprintf(stderr,                                                    \
                    " %s:%02d " GSHL_FG_CYAN(" %-30s ") " ... " GSHL_FG_RED(   \
                        " FAILED ") " Assertion failed : %s == %s\n",          \
                    __FILE__, __LINE__, __func__, #EXPR1, #EXPR2);             \
                                                                               \
            if (opt.continue_on_fail) {                                        \
                return;                                                        \
            }                                                                  \
        }                                                                      \
        else {                                                                 \
            fprintf(stderr,                                                    \
                    " %s:%02d " GSHL_FG_CYAN(" %-30s ") " ... " GSHL_FG_GREEN( \
                        " PASSED ") "\n",                                      \
                    __FILE__, __LINE__, __func__);                             \
        }                                                                      \
    } while (0)

#define GSHL_TEST_NEQUAL(EXPR1, EXPR2, ...)                                    \
    do {                                                                       \
        GSHL_TestEqualOpt opt = (GSHL_TestEqualOpt){__VA_ARGS__};              \
                                                                               \
        if ((EXPR1) == (EXPR2)) {                                              \
            *failed_tests += 1;                                                \
            fprintf(stderr,                                                    \
                    " %s:%02d " GSHL_FG_CYAN(" %-30s ") " ... " GSHL_FG_RED(   \
                        " FAILED ") " Assertion failed : %s != %s\n",          \
                    __FILE__, __LINE__, __func__, #EXPR1, #EXPR2);             \
                                                                               \
            if (opt.continue_on_fail) {                                        \
                return;                                                        \
            }                                                                  \
        }                                                                      \
        else {                                                                 \
            fprintf(stderr,                                                    \
                    " %s:%02d " GSHL_FG_CYAN(" %-30s ") " ... " GSHL_FG_GREEN( \
                        " PASSED ") "\n",                                      \
                    __FILE__, __LINE__, __func__);                             \
        }                                                                      \
    } while (0)

#define GSHL_TEST_STR_EQUAL(STR1, STR2, ...)                                   \
    do {                                                                       \
        GSHL_TestEqualOpt opt = (GSHL_TestEqualOpt){__VA_ARGS__};              \
                                                                               \
        if (strcmp((STR1), (STR2)) != 0) {                                     \
            *failed_tests += 1;                                                \
            if (opt.align_on_new_line) {                                       \
                fprintf(                                                       \
                    stderr,                                                    \
                    " %s:%02d " GSHL_FG_CYAN(" %-30s ") " ... " GSHL_FG_RED(   \
                        " FAILED ") " Assertion failed : \n\"%s\" == "         \
                                    "\n\"%s\"\n",                              \
                    __FILE__, __LINE__, __func__, STR1, STR2);                 \
            }                                                                  \
            else {                                                             \
                fprintf(                                                       \
                    stderr,                                                    \
                    " %s:%02d " GSHL_FG_CYAN(" %-30s ") " ... " GSHL_FG_RED(   \
                        " FAILED ") " Assertion failed : \"%s\" == \"%s\"\n",  \
                    __FILE__, __LINE__, __func__, STR1, STR2);                 \
            }                                                                  \
                                                                               \
            if (opt.continue_on_fail) {                                        \
                return;                                                        \
            }                                                                  \
        }                                                                      \
        else {                                                                 \
            fprintf(stderr,                                                    \
                    " %s:%02d " GSHL_FG_CYAN(" %-30s ") " ... " GSHL_FG_GREEN( \
                        " PASSED ") "\n",                                      \
                    __FILE__, __LINE__, __func__);                             \
        }                                                                      \
    } while (0)

/// }}}

/// {{{ Types

typedef void(GSHL_TestFunction)(usize *failed_tests);

typedef struct GSHL_Test {
    char *name;
    GSHL_TestFunction *func;
    struct GSHL_Test *next;
    struct GSHL_Test *prev;
} GSHL_Test;

typedef struct GSHL_TestEqualOpt {
    bool continue_on_fail;
    bool align_on_new_line;
} GSHL_TestEqualOpt;

/// }}}

/// {{{ Functions

GSHLDEF void GSHL_register_test_wrapper(const char *name,
                                        GSHL_TestFunction test_func);
GSHLDEF usize GSHL_run_tests(const char *filter);

/// }}}

#ifdef GSHL_STRIP_PREFIX
#    define TEST_EQUAL GSHL_TEST_EQUAL
#    define REGISTER_TEST GSHL_REGISTER_TEST
#    define run_tests GSHL_run_tests
#endif

// #endif // INCLUDE_TEST_MOD_H_
// #ifndef INCLUDE_PRINT_MOD_H_
// #define INCLUDE_PRINT_MOD_H_

// #include "format/mod.h"
// #include "types/mod.h"

#include <pthread.h>
#include <stdarg.h>

/// {{{ Types

typedef struct GSHL_GlobalFormatString {
    struct {
        GSHL_FormatString string;
        Fd fd;
    } buffers[2];
    pthread_t writer_tid;
    pthread_mutex_t lock;
    pthread_cond_t notify;
    bool init;
    bool ready;
} GSHL_GlobalFormatString;

/// }}}

/// {{{ Functions

usize GSHL_print(const char *const restrict format, ...);
usize GSHL_dprint(const int fd, const char *const restrict format, ...);

usize GSHL_println(const char *const restrict format, ...);
usize GSHL_dprintln(const int fd, const char *const restrict format, ...);

usize GSHL_printv(const char *const restrict format, va_list args);
usize GSHL_dprintv(const int fd, const char *const restrict format,
                   va_list args);

usize GSHL_printlnv(const char *const restrict format, va_list args);
usize GSHL_dprintlnv(const int fd, const char *const restrict format,
                     va_list args);

/// }}}

#ifdef GSHL_STRIP_PREFIX
#    define print GSHL_print
#    define printv GSHL_printv
#    define println GSHL_println
#    define printlnv GSHL_printlnv
#    define dprint GSHL_dprint
#    define dprintv GSHL_dprintv
#    define dprintln GSHL_dprintln
#    define dprintlnv GSHL_dprintlnv
#endif

// #endif // INCLUDE_PRINT_MOD_H_
// #ifndef INCLUDE_MEM_MOD_H_
// #define INCLUDE_MEM_MOD_H_

// #include "types/mod.h"

/// {{{ Functions

void *GSHL_memdup(const void *const restrict source, const usize size);

/// }}}

#ifdef GSHL_STRIP_PREFIX
#    define memdup GSHL_memdup
#endif

// #endif // INCLUDE_MEM_MOD_H_
// #ifndef INCLUDE_MATH_MOD_H_
// #define INCLUDE_MATH_MOD_H_

// #include "types/mod.h"

/// {{{ Functions

usize GSHL_factorial(const usize number);
bool GSHL_is_power_of_two(const usize number);

/// }}}

#ifdef GSHL_STRIP_PREFIX
#    define factorial GSHL_factorial
#    define is_power_of_two GSHL_is_power_of_two
#endif

// #endif // INCLUDE_MATH_MOD_H_
// #ifndef INCLUDE_LOG_MOD_H_
// #define INCLUDE_LOG_MOD_H_

// #include "format/mod.h"
// #include "types/mod.h"

#include <stdarg.h>

/// {{{ Macros

#define GSHL_LOG_VERBOSITY_LEVELS                                              \
    /* VERBOSITY(ENUM, BIT_SHIFT, FORMAT, ...) */                              \
    VERBOSITY(DEBUG, 0, GSHL_FG_GREEN("%s"), "DEBUG", "debug")                 \
    VERBOSITY(INFO, 1, GSHL_FG_CYAN("%s"), "INFO", "info")                     \
    VERBOSITY(WARNING, 2, GSHL_FG_YELLOW("%s"), "WARNING", "warning")          \
    VERBOSITY(ERROR, 3, GSHL_FG_RED("%s"), "ERROR", "error")

#define GSHL_log(KIND, ...)                                                    \
    GSHL_log_wrapper(KIND,                                                     \
                     (GSHL_LogOpts){                                           \
                         .pretty_function = __PRETTY_FUNCTION__,               \
                         .function = __FUNCTION__,                             \
                         .file = __FILE__,                                     \
                         .line = __LINE__,                                     \
                     },                                                        \
                     __VA_ARGS__)

#define GSHL_log_init(...) GSHL_log_init_wrapper((GSHL_LogConfig){__VA_ARGS__})

/// }}}

/// {{{ Types

typedef enum GSHL_LogKind {
    GSHL_LOG_NONE = 0,
#define VERBOSITY(ENUM, BIT_SHIFT, FORMAT, ...)                                \
    GSHL_LOG_##ENUM = 1U << BIT_SHIFT,
    GSHL_LOG_VERBOSITY_LEVELS
#undef VERBOSITY

        GSHL_LOG_ALL = 0
#define VERBOSITY(ENUM, BIT_SHIFT, FORMAT, ...) | GSHL_LOG_##ENUM
    GSHL_LOG_VERBOSITY_LEVELS
#undef VERBOSITY
    ,
} GSHL_LogKind;

typedef struct GSHL_LogOpts {
    const char *pretty_function;
    const char *function;
    const char *file;
    usize line;
} GSHL_LogOpts;

typedef struct GSHL_LogConfig {
    Fd fd;
    GSHL_LogKind mask;
    Fd file_fd;
    struct {
        enum GSHL_LogPrintFunction {
            GSHL_LOG_PRINT_FUNCTION = 0,
            GSHL_LOG_PRINT_PRETTY_FUNCTION = 1,
        } function : 1;
        bool file : 1;
    } print;
} GSHL_LogConfig;

/// }}}

/// {{{ Functions

void GSHL_log_init_wrapper(const GSHL_LogConfig config);
usize GSHL_log_wrapper(const GSHL_LogKind kind, const GSHL_LogOpts opts,
                       const char *const restrict format, ...);
GSHL_LogConfig GSHL_log_get_config(void);
void GSHL_log_read_env(void);
void GSHL_log_print_config(void);

/// }}}

#ifdef GSHL_STRIP_PREFIX
#    define LogKind GSHL_LogKind
#    define LogConfig GSHL_LogConfig
#    define log GSHL_log
#    define log_init GSHL_log_init
#    define log_get_config GSHL_log_get_config
#    define log_read_env GSHL_log_read_env
#    define VERBOSITY(ENUM, BIT_SHIFT, FORMAT, ...)                            \
        static const GSHL_LogKind ENUM = GSHL_LOG_##ENUM;
GSHL_LOG_VERBOSITY_LEVELS
#    undef VERBOSITY
#    define log_print_config GSHL_log_print_config
#endif

// #endif // INCLUDE_LOG_MOD_H_
// #ifndef INCLUDE_HASH_MOD_H_
// #define INCLUDE_HASH_MOD_H_

// #endif // INCLUDE_HASH_MOD_H_
// #ifndef INCLUDE_HASH_KR_H_
// #define INCLUDE_HASH_KR_H_

// #include "types/mod.h"

/// {{{ Functions

usize GSHL_hash_kr(const char *const string);

/// }}}

#ifdef GSHL_STRIP_PREFIX
#    define hash_kr GSHL_hash_kr
#endif

// #endif // INCLUDE_HASH_KR_H_
// #ifndef INCLUDE_HASH_FNV1A_H_
// #define INCLUDE_HASH_FNV1A_H_

// #include "types/mod.h"

/// {{{ Functions

u32 GSHL_hash_fnv1a32(const char *const input);

/// }}}

#ifdef GSHL_STRIP_PREFIX
#    define hash_fnv1a32 GSHL_hash_fnv1a32
#endif

// #endif // INCLUDE_HASH_FNV1A_H_
// #ifndef INCLUDE_HASH_DJB2_H_
// #define INCLUDE_HASH_DJB2_H_

// #include "types/mod.h"

/// {{{ Functions

usize GSHL_hash_djb2(const char *string);

/// }}}

#ifdef GSHL_STRIP_PREFIX
#    define hash_djb2 GSHL_hash_djb2
#endif

// #endif // INCLUDE_HASH_DJB2_H_
// #ifndef INCLUDE_FORMAT_USIZE_H_
// #define INCLUDE_FORMAT_USIZE_H_

// #include "format/mod.h"

/// {{{ Functions

GSHL_FormatSpecifierWriteFunction GSHL_write_usize;

/// }}}

// #endif // INCLUDE_FORMAT_USIZE_H_
// #ifndef INCLUDE_FORMAT_U8_H_
// #define INCLUDE_FORMAT_U8_H_

// #include "format/mod.h"

/// {{{ Functions

GSHL_FormatSpecifierWriteFunction GSHL_write_u8;

/// }}}

// #endif // INCLUDE_FORMAT_U8_H_
 
// #ifndef INCLUDE_FORMAT_U64_H_
// #define INCLUDE_FORMAT_U64_H_

// #include "format/mod.h"

/// {{{ Functions

GSHL_FormatSpecifierWriteFunction GSHL_write_u64;

/// }}}

// #endif // INCLUDE_FORMAT_U64_H_
// #ifndef INCLUDE_FORMAT_U32_H_
// #define INCLUDE_FORMAT_U32_H_

// #include "format/mod.h"

/// {{{ Functions

GSHL_FormatSpecifierWriteFunction GSHL_write_u32;

/// }}}

// #endif // INCLUDE_FORMAT_U32_H_
// #ifndef INCLUDE_FORMAT_U16_H_
// #define INCLUDE_FORMAT_U16_H_

// #include "format/mod.h"

/// {{{ Functions

GSHL_FormatSpecifierWriteFunction GSHL_write_u16;

/// }}}

// #endif // INCLUDE_FORMAT_U16_H_
// #ifndef INCLUDE_FORMAT_POINTER_H_
// #define INCLUDE_FORMAT_POINTER_H_

// #include "format/mod.h"

/// {{{ Functions

GSHL_FormatSpecifierWriteFunction GSHL_write_pointer;

/// }}}

// #endif // INCLUDE_FORMAT_POINTER_H_
// #ifndef INCLUDE_FORMAT_ISIZE_H_
// #define INCLUDE_FORMAT_ISIZE_H_

// #include "format/mod.h"

/// {{{ Functions

GSHL_FormatSpecifierWriteFunction GSHL_write_isize;

/// }}}

// #endif // INCLUDE_FORMAT_ISIZE_H_
// #ifndef INCLUDE_FORMAT_I8_H_
// #define INCLUDE_FORMAT_I8_H_

// #include "format/mod.h"

/// {{{ Functions

GSHL_FormatSpecifierWriteFunction GSHL_write_i8;

/// }}}

// #endif // INCLUDE_FORMAT_I8_H_
// #ifndef INCLUDE_FORMAT_I64_H_
// #define INCLUDE_FORMAT_I64_H_

// #include "format/mod.h"

/// {{{ Functions

GSHL_FormatSpecifierWriteFunction GSHL_write_i64;

/// }}}

// #endif // INCLUDE_FORMAT_I64_H_
// #ifndef INCLUDE_FORMAT_I32_H_
// #define INCLUDE_FORMAT_I32_H_

// #include "format/mod.h"

/// {{{ Functions

GSHL_FormatSpecifierWriteFunction GSHL_write_i32;

/// }}}

// #endif // INCLUDE_FORMAT_I32_H_
// #ifndef INCLUDE_FORMAT_I16_H_
// #define INCLUDE_FORMAT_I16_H_

// #include "format/mod.h"

/// {{{ Functions

GSHL_FormatSpecifierWriteFunction GSHL_write_i16;

/// }}}

// #endif // INCLUDE_FORMAT_I16_H_
// #ifndef INCLUDE_FORMAT_HEX_H_
// #define INCLUDE_FORMAT_HEX_H_

// #include "format/mod.h"

/// {{{ Functions

GSHL_FormatSpecifierWriteFunction GSHL_write_hex32;
GSHL_FormatSpecifierWriteFunction GSHL_write_hex64;

/// }}}

// #endif // INCLUDE_FORMAT_HEX_H_
// #ifndef INCLUDE_FORMAT_CSTRING_H_
// #define INCLUDE_FORMAT_CSTRING_H_

// #include "format/mod.h"

/// {{{ Functions

GSHL_FormatSpecifierWriteFunction GSHL_write_cstring;

/// }}}

// #endif // INCLUDE_FORMAT_CSTRING_H_
// #ifndef INCLUDE_FORMAT_CHARACTER_H_
// #define INCLUDE_FORMAT_CHARACTER_H_

// #include "format/mod.h"

/// {{{ Functions

GSHL_FormatSpecifierWriteFunction GSHL_write_char;

/// }}}

// #endif // INCLUDE_FORMAT_CHARACTER_H_
// #ifndef INCLUDE_FORMAT_BOOL_H_
// #define INCLUDE_FORMAT_BOOL_H_

// #include "format/mod.h"

/// {{{ Functions

GSHL_FormatSpecifierWriteFunction GSHL_write_bool;

/// }}}

// #endif // INCLUDE_FORMAT_BOOL_H_
// #ifndef INCLUDE_ARRAY_MOD_H_
// #define INCLUDE_ARRAY_MOD_H_

// #include "types/mod.h"

/// {{{ Macros

#define GSHL_ARRAYN_PRINT(ARRAY, N, ELEMENT_FORMAT, ...)                       \
    do {                                                                       \
        struct GSHL_ArrayPrintOpts opts = {__VA_ARGS__};                       \
        opts.sep = opts.sep ? opts.sep : ", ";                                 \
        opts.end = opts.end ? opts.end : "\n";                                 \
        opts.array_limits = opts.array_limits ? opts.array_limits : "{}";      \
        printf("%s[%zu] = %c ", #ARRAY, N, opts.array_limits[0]);              \
        for (usize array_index = 0; array_index < N; ++array_index) {          \
            if (array_index == 0) {                                            \
                printf(ELEMENT_FORMAT, ARRAY[array_index]);                    \
            }                                                                  \
            else {                                                             \
                printf("%s" ELEMENT_FORMAT, opts.sep, ARRAY[array_index]);     \
            }                                                                  \
        }                                                                      \
        printf(" %c%s", opts.array_limits[1], opts.end);                       \
    } while (0)
#define GSHL_ARRAYN_FOREACH(ARRAY, N, VARDECL, ...)                            \
    for (usize keep = 1, index = 0, size = (N); keep && index < size;          \
         keep = !keep, index++)                                                \
        for (const VARDECL = (ARRAY)[index]; keep; keep = !keep)
#define GSHL_ARRAYN_FOREACH_MUT(ARRAY, N, VARDECL)                             \
    for (usize keep = 1, index = 0, size = (N); keep && index < size;          \
         keep = !keep, index++)                                                \
        for (VARDECL = &((ARRAY)[index]); keep; keep = !keep)

#define GSHL_ARRAY_LEN(ARRAY) (sizeof(ARRAY) / sizeof(ARRAY[0]))
#define GSHL_ARRAY_PRINT(ARRAY, ELEMENT_FORMAT, ...)                           \
    GSHL_ARRAYN_PRINT(ARRAY, GSHL_ARRAY_LEN(ARRAY), ELEMENT_FORMAT, __VA_ARGS__)
#define GSHL_ARRAY_FOREACH(ARRAY, VARDECL, ...)                                \
    GSHL_ARRAYN_FOREACH(ARRAY, GSHL_ARRAY_LEN(ARRAY), VARDECL, __VA_ARGS__)
#define GSHL_ARRAY_FOREACH_MUT(ARRAY, VARDECL)                                 \
    GSHL_ARRAYN_FOREACH_MUT(ARRAY, GSHL_ARRAY_LEN(ARRAY), VARDECL)

/// }}}

/// {{{ Types

typedef struct GSHL_ArrayPrintOpts {
    char *sep;
    char *end;
    char *array_limits;
} GSHL_ArrayPrintOpts;

typedef struct GSHL_ArrayForEachOpts {
    usize length;

    u8 unused[];
} GSHL_ArrayForEachOpts;

/// }}}

/// {{{ Functions

/// }}}

/// {{{ Strip Prefix

#ifdef GSHL_STRIP_PREFIX
#    define ARRAY_LEN GSHL_ARRAY_LEN
#    define ARRAY_PRINT GSHL_ARRAY_PRINT
#    define ARRAY_FOREACH GSHL_ARRAY_FOREACH
#    define ARRAY_FOREACH_MUT GSHL_ARRAY_FOREACH_MUT
#    define ARRAYN_PRINT GSHL_ARRAYN_PRINT
#    define ARRAYN_FOREACH GSHL_ARRAYN_FOREACH
#    define ARRAYN_FOREACH_MUT GSHL_ARRAYN_FOREACH_MUT
#endif

/// }}}

// #endif // INCLUDE_ARRAY_MOD_H_
// #ifndef INCLUDE_ANSI_MOD_H_
// #define INCLUDE_ANSI_MOD_H_

// #include "ansi/colors.h" // IWYU pragma: export

// #endif // INCLUDE_ANSI_MOD_H_
// #ifndef INCLUDE_ANSI_COLORS_H_
// #define INCLUDE_ANSI_COLORS_H_

/// {{{ Macros

#define GSHL_FG_BLACK(TEXT) "\033[30m" TEXT "\033[0m"
#define GSHL_FG_RED(TEXT) "\033[31m" TEXT "\033[0m"
#define GSHL_FG_GREEN(TEXT) "\033[32m" TEXT "\033[0m"
#define GSHL_FG_YELLOW(TEXT) "\033[33m" TEXT "\033[0m"
#define GSHL_FG_BLUE(TEXT) "\033[34m" TEXT "\033[0m"
#define GSHL_FG_MAGENTA(TEXT) "\033[35m" TEXT "\033[0m"
#define GSHL_FG_CYAN(TEXT) "\033[36m" TEXT "\033[0m"
#define GSHL_FG_WHITE(TEXT) "\033[37m" TEXT "\033[0m"

#define GSHL_BG_BLACK(TEXT) "\033[40m" TEXT "\033[0m"
#define GSHL_BG_RED(TEXT) "\033[41m" TEXT "\033[0m"
#define GSHL_BG_GREEN(TEXT) "\033[42m" TEXT "\033[0m"
#define GSHL_BG_YELLOW(TEXT) "\033[43m" TEXT "\033[0m"
#define GSHL_BG_BLUE(TEXT) "\033[44m" TEXT "\033[0m"
#define GSHL_BG_MAGENTA(TEXT) "\033[45m" TEXT "\033[0m"
#define GSHL_BG_CYAN(TEXT) "\033[46m" TEXT "\033[0m"
#define GSHL_BG_WHITE(TEXT) "\033[47m" TEXT "\033[0m"

/// }}}

/// {{{ Strip Prefix

#ifdef GSHL_STRIP_PREFIX
#    define FG_BLACK(...) GSHL_FG_BLACK(__VA_ARGS__)
#    define FG_RED(...) GSHL_FG_RED(__VA_ARGS__)
#    define FG_GREEN(...) GSHL_FG_GREEN(__VA_ARGS__)
#    define FG_YELLOW(...) GSHL_FG_YELLOW(__VA_ARGS__)
#    define FG_BLUE(...) GSHL_FG_BLUE(__VA_ARGS__)
#    define FG_MAGENTA(...) GSHL_FG_MAGENTA(__VA_ARGS__)
#    define FG_CYAN(...) GSHL_FG_CYAN(__VA_ARGS__)
#    define FG_WHITE(...) GSHL_FG_WHITE(__VA_ARGS__)

#    define BG_BLACK(...) GSHL_BG_BLACK(__VA_ARGS__)
#    define BG_RED(...) GSHL_BG_RED(__VA_ARGS__)
#    define BG_GREEN(...) GSHL_BG_GREEN(__VA_ARGS__)
#    define BG_YELLOW(...) GSHL_BG_YELLOW(__VA_ARGS__)
#    define BG_BLUE(...) GSHL_BG_BLUE(__VA_ARGS__)
#    define BG_MAGENTA(...) GSHL_BG_MAGENTA(__VA_ARGS__)
#    define BG_CYAN(...) GSHL_BG_CYAN(__VA_ARGS__)
#    define BG_WHITE(...) GSHL_BG_WHITE(__VA_ARGS__)
#endif

/// }}}

// #endif // INCLUDE_ANSI_COLORS_H_
#endif // INCLUDE_GSHL_H_
#ifdef GSHL_IMPLEMENTATION
#ifdef GSHL_SOURCE_CODE_MAPPING
#    line 0 "src/types/mod.c"
#endif // GSHL_SOURCE_CODE_MAPPING
// #include "types/mod.h"

#ifdef GSHL_TESTS

// #    include "test/mod.h"

GSHL_TEST(types_size)
{
    GSHL_TEST_EQUAL(sizeof(u8), 1);
    GSHL_TEST_EQUAL(sizeof(u16), 2);
    GSHL_TEST_EQUAL(sizeof(u32), 4);
    GSHL_TEST_EQUAL(sizeof(u64), 8);
    GSHL_TEST_EQUAL(sizeof(usize), 8);

    GSHL_TEST_EQUAL(sizeof(i8), 1);
    GSHL_TEST_EQUAL(sizeof(i16), 2);
    GSHL_TEST_EQUAL(sizeof(i32), 4);
    GSHL_TEST_EQUAL(sizeof(i64), 8);
    GSHL_TEST_EQUAL(sizeof(isize), 8);

    GSHL_TEST_EQUAL(sizeof(bool), 1);

    GSHL_TEST_EQUAL(sizeof(f32), 4);
    GSHL_TEST_EQUAL(sizeof(f64), 8);
}

#endif
#ifdef GSHL_SOURCE_CODE_MAPPING
#    line 0 "src/types/limits.c"
#endif // GSHL_SOURCE_CODE_MAPPING
// #include "types/limits.h"

#ifdef GSHL_TESTS
// #    include "test/mod.h"

GSHL_TEST(types_limits)
{
    GSHL_TEST_EQUAL((u8)(U8_MAX + 1), 0);
    GSHL_TEST_EQUAL((u8)(U8_MIN - 1), U8_MAX);

    GSHL_TEST_EQUAL((u16)(U16_MAX + 1), 0);
    GSHL_TEST_EQUAL((u16)(U16_MIN - 1), U16_MAX);

    GSHL_TEST_EQUAL((u32)(U32_MAX + 1), 0);
    GSHL_TEST_EQUAL((u32)(U32_MIN - 1), U32_MAX);
}

#endif
#ifdef GSHL_SOURCE_CODE_MAPPING
#    line 0 "src/test/mod.c"
#endif // GSHL_SOURCE_CODE_MAPPING
// #include "test/mod.h"
// #include "macros/mod.h"
// #include "types/mod.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static GSHL_Test *GSHL_test_list = NULL;

GSHLDEF usize GSHL_run_tests(const char *filter)
{
    usize failed_tests = 0;
    size_t total = 0;

    printf("Running tests...\n");

    // TODO: Remove O(n)
    GSHL_Test *last = NULL;
    for (last = GSHL_test_list; last != NULL && last->next != NULL;
         last = last->next) {
    }

    /* Reverse Order */
    for (GSHL_Test *current = last; current != NULL; current = current->prev) {
        if (filter != NULL && strstr(current->name, filter) == NULL) {
            continue;
        }

        current->func(&failed_tests);
        total++;
    }

    printf("\n");
    if (failed_tests != 0) {
        printf(GSHL_FG_RED("Failed tests:") " %zu\n", failed_tests);
    }
    printf("Total Tests:  %zu\n", total);

    return failed_tests;
}

GSHLDEF void GSHL_register_test_wrapper(const char *name,
                                        GSHL_TestFunction test_func)
{
    GSHL_Test *new_test = malloc(sizeof(*new_test));

    if (GSHL_test_list != NULL) {
        GSHL_test_list->prev = new_test;
    }

    new_test->name = (char *)name;
    new_test->func = test_func;
    new_test->next = GSHL_test_list;
    new_test->prev = NULL;

    GSHL_test_list = new_test;
}

int test_main(int argc, char *argv[])
{
    GSHL_UNUSED(argc);
    GSHL_UNUSED(argv);

    GSHL_run_tests(NULL);

    return 0;
}
#ifdef GSHL_SOURCE_CODE_MAPPING
#    line 0 "src/string/mod.c"
#endif // GSHL_SOURCE_CODE_MAPPING
// #include "string/mod.h"
#include <stdlib.h>
#include <string.h>

char *GSHL_string_dup(const char *const source)
{
    const usize len = strlen(source);
    return strcpy(malloc(len + 1), source);
}

void GSHL_string_reverse(char *start, char *end)
{
    end -= 1;

    while (start < end) {
        char temp = *start;
        *start = *end;
        *end = temp;

        start++;
        end--;
    }
}

#ifdef GSHL_TESTS
// #    include "test/mod.h"

GSHL_TEST(string_dup)
{
    {
        char *s = string_dup("example");
        GSHL_TEST_STR_EQUAL(s, "example");
        free(s);
    }

    {
        char *s = string_dup("Hello world!");
        GSHL_TEST_STR_EQUAL(s, "Hello world!");
        free(s);
    }
}

GSHL_TEST(string_reverse)
{
    {
        char s1[] = "123456789";
        char s2[] = "123456789";
        GSHL_string_reverse(s2, s2 + GSHL_STACK_STRING_LEN(s2));
        GSHL_TEST_STR_EQUAL(s2, "987654321");
        GSHL_string_reverse(s2, s2 + GSHL_STACK_STRING_LEN(s2));
        GSHL_TEST_STR_EQUAL(s1, s2);
    }
}

#endif
#ifdef GSHL_SOURCE_CODE_MAPPING
#    line 0 "src/print/mod.c"
#endif // GSHL_SOURCE_CODE_MAPPING
// #include "print/mod.h"
// #include "format/mod.h"
// #include "string/mod.h"

#include <stdarg.h>
#include <unistd.h>

#ifdef GSHL_GLOBAL_FORMAT_STRING
// #    include "array/mod.h"
// #    include "macros/mod.h"

#    include <assert.h>
#    include <stdlib.h>
#    include <string.h>

static GSHL_GlobalFormatString global_string = {};

extern int usleep(__useconds_t usec);

static void *GSHL_global_format_string_writer_thread(void *arg)
{
    GSHL_UNUSED(arg);

    while (true) {
        pthread_mutex_lock(&global_string.lock);
        {
            while (!global_string.ready) {
                pthread_cond_wait(&global_string.notify, &global_string.lock);
            }

            for (usize fd = STDOUT_FILENO;
                 fd < GSHL_ARRAY_LEN(global_string.buffers); ++fd) {
                GSHL_FormatString *string = &global_string.buffers[fd].string;

                GSHL_ASSERT(fd > 0);

                if (string->count == 0) {
                    continue;
                }

                assert(write(fd, string->items, string->count) > 0);
                GSHL_DArray_init(string);
            }

            // Reset the buffer state
            global_string.ready = false;
        }
        pthread_mutex_unlock(&global_string.lock);

        usleep(10 * 1000);
    }

    return NULL;
}

static __attribute__((constructor)) void
GSHL_global_format_string_constructor(void)
{
    pthread_mutex_init(&global_string.lock, NULL);
    pthread_cond_init(&global_string.notify, NULL);
    pthread_create(&global_string.writer_tid, NULL,
                   GSHL_global_format_string_writer_thread, NULL);

    pthread_mutex_lock(&global_string.lock);
    for (usize fd = STDOUT_FILENO; fd < GSHL_ARRAY_LEN(global_string.buffers);
         ++fd) {
        GSHL_FormatString *string = &global_string.buffers[fd].string;
        GSHL_DArray_init(string);

        GSHL_ASSERT(fd > 0);
    }
    pthread_mutex_unlock(&global_string.lock);
}

static __attribute__((destructor)) void
GSHL_global_format_string_destructor(void)
{
    pthread_mutex_lock(&global_string.lock);
    for (usize fd = STDOUT_FILENO; fd < GSHL_ARRAY_LEN(global_string.buffers);
         ++fd) {
        GSHL_FormatString *string = &global_string.buffers[fd].string;

        GSHL_ASSERT(fd > 0);

        if (string->count > 0) {
            assert(write(fd, string->items, string->count) > 0);
        }

        GSHL_DArray_destroy(string);
    }
    pthread_mutex_unlock(&global_string.lock);

    // Cleanup
    pthread_cancel(global_string.writer_tid);
    pthread_join(global_string.writer_tid, NULL);

    pthread_mutex_destroy(&global_string.lock);
    pthread_cond_destroy(&global_string.notify);
}
#endif

usize GSHL_print(const char *const restrict format, ...)
{
    va_list args;
    va_start(args, format);
    const usize count = GSHL_printv(format, args);
    va_end(args);
    return count;
}

usize GSHL_dprint(const int fd, const char *const restrict format, ...)
{
    va_list args;
    va_start(args, format);
    const usize count = GSHL_dprintv(fd, format, args);
    va_end(args);
    return count;
}

usize GSHL_println(const char *const restrict format, ...)
{
    va_list args;
    va_start(args, format);
    const usize count = GSHL_printlnv(format, args);
    va_end(args);
    return count;
}

usize GSHL_dprintln(const int fd, const char *const restrict format, ...)
{
    va_list args;
    va_start(args, format);
    const usize count = GSHL_dprintlnv(fd, format, args);
    va_end(args);
    return count;
}

usize GSHL_printv(const char *const restrict format, va_list args)
{
    return GSHL_dprintv(STDOUT_FILENO, format, args);
}

usize GSHL_dprintv(const int fd, const char *const restrict format,
                   va_list args)
{
    if (fd <= 0) {
        return 0;
    }

#ifdef GSHL_GLOBAL_FORMAT_STRING
    pthread_mutex_lock(&global_string.lock);
    usize count = 0;
    for (usize i = STDOUT_FILENO; i < GSHL_ARRAY_LEN(global_string.buffers);
         ++i) {
        global_string.buffers[i].fd = i;
        GSHL_FormatString *string = &global_string.buffers[i].string;
        const Fd global_fd = global_string.buffers[i].fd;

        GSHL_ASSERT(global_fd > 0);

        if (global_fd != fd) {
            continue;
        }

        const usize start_count = string->count;
        GSHL_format_writev(string, format, args);
        count = string->count - start_count;
        global_string.ready = true;                 // Mark buffer as ready
        pthread_cond_signal(&global_string.notify); // Signal writer thread

        break;
    }
    pthread_mutex_unlock(&global_string.lock);

    if (count != 0) {
        return count;
    }
#endif
    const GSHL_StringView view = GSHL_format_wrapperv(format, args);

    return write(fd, view.start, view.len);
}

usize GSHL_printlnv(const char *const restrict format, va_list args)
{
    return GSHL_dprintlnv(STDOUT_FILENO, format, args);
}

usize GSHL_dprintlnv(const int fd, const char *const restrict format,
                     va_list args)
{
    if (fd <= 0) {
        return 0;
    }

#ifdef GSHL_GLOBAL_FORMAT_STRING
    pthread_mutex_lock(&global_string.lock);
    usize count = 0;
    for (usize i = STDOUT_FILENO; i < GSHL_ARRAY_LEN(global_string.buffers);
         ++i) {
        global_string.buffers[i].fd = i;
        GSHL_FormatString *string = &global_string.buffers[i].string;
        const Fd global_fd = global_string.buffers[i].fd;

        GSHL_ASSERT(global_fd > 0);

        if (global_fd != fd) {
            continue;
        }

        const usize start_count = string->count;
        GSHL_format_writev(string, format, args);
        GSHL_format_write(string, "\n");
        count = string->count - start_count;
        global_string.ready = true;                 // Mark buffer as ready
        pthread_cond_signal(&global_string.notify); // Signal writer thread

        break;
    }
    pthread_mutex_unlock(&global_string.lock);

    if (count != 0) {
        return count;
    }
#endif

    const GSHL_StringView view = GSHL_format_wrapperv(format, args);

    return write(fd, view.start, view.len) + write(fd, "\n", 1);
}
#ifdef GSHL_SOURCE_CODE_MAPPING
#    line 0 "src/math/mod.c"
#endif // GSHL_SOURCE_CODE_MAPPING
// #include "math/mod.h"

usize GSHL_factorial(const usize number)
{
    usize result = 1;
    for (usize n = 2; n <= number; ++n) {
        result *= n;
    }

    return result;
}

bool GSHL_is_power_of_two(const usize number)
{
    return (number > 0) && !(number & (number - 1));
}

#ifdef GSHL_TESTS

// #    include "test/mod.h"

GSHL_TEST(factorial)
{
    GSHL_TEST_EQUAL(GSHL_factorial(0), 1);
    GSHL_TEST_EQUAL(GSHL_factorial(1), 1);
    GSHL_TEST_EQUAL(GSHL_factorial(2), 2);
    GSHL_TEST_EQUAL(GSHL_factorial(3), 6);
    GSHL_TEST_EQUAL(GSHL_factorial(4), 24);
    GSHL_TEST_EQUAL(GSHL_factorial(5), 120);
    GSHL_TEST_EQUAL(GSHL_factorial(6), 720);
    GSHL_TEST_EQUAL(GSHL_factorial(7), 5040);
}

GSHL_TEST(is_power_of_two)
{
    GSHL_TEST_EQUAL(GSHL_is_power_of_two(0), false);
    GSHL_TEST_EQUAL(GSHL_is_power_of_two(1), true);
    GSHL_TEST_EQUAL(GSHL_is_power_of_two(2), true);
    GSHL_TEST_EQUAL(GSHL_is_power_of_two(3), false);
    GSHL_TEST_EQUAL(GSHL_is_power_of_two(4), true);
    GSHL_TEST_EQUAL(GSHL_is_power_of_two(5), false);
    GSHL_TEST_EQUAL(GSHL_is_power_of_two(6), false);
    GSHL_TEST_EQUAL(GSHL_is_power_of_two(7), false);
    GSHL_TEST_EQUAL(GSHL_is_power_of_two(8), true);
    GSHL_TEST_EQUAL(GSHL_is_power_of_two(9), false);
    GSHL_TEST_EQUAL(GSHL_is_power_of_two(10), false);
    GSHL_TEST_EQUAL(GSHL_is_power_of_two(11), false);
    GSHL_TEST_EQUAL(GSHL_is_power_of_two(12), false);
    GSHL_TEST_EQUAL(GSHL_is_power_of_two(13), false);
    GSHL_TEST_EQUAL(GSHL_is_power_of_two(14), false);
    GSHL_TEST_EQUAL(GSHL_is_power_of_two(15), false);
    GSHL_TEST_EQUAL(GSHL_is_power_of_two(16), true);

    for (u8 i = 0; i < 64; ++i) {
        GSHL_TEST_EQUAL(GSHL_is_power_of_two(1UL << i), true);
    }
}

#endif
#ifdef GSHL_SOURCE_CODE_MAPPING
#    line 0 "src/log/mod.c"
#endif // GSHL_SOURCE_CODE_MAPPING
// #include "log/mod.h"

// #include "ansi/colors.h"
// #include "array/mod.h"
// #include "format/mod.h"
// #include "macros/mod.h"
// #include "math/mod.h"
// #include "print/mod.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static GSHL_LogConfig log_config = {
    .fd = STDERR_FILENO,
    .mask = GSHL_LOG_ALL,
    .file_fd = 0,
    .print =
        {
            .file = true,
            .function = GSHL_LOG_PRINT_FUNCTION,
        },
};

usize write_enum_LogKind(struct GSHL_FormatString *string,
                         const struct GSHL_FormatSpecifier *fs)
{
    GSHL_ASSERT(fs != NULL);

    GSHL_LogKind log = fs->value.u32;

    if (log == 0) {
        return GSHL_format_write(string, "NONE");
    }

    usize count = 0;
    for (GSHL_LogKind mask = 1U << 0; log > 0; log &= ~mask, mask <<= 1) {
        const GSHL_LogKind enabled_log = log & mask;

        if (count > 0 && enabled_log) {
            count += GSHL_format_write(string, " | ");
        }

        switch (enabled_log) {
#define VERBOSITY(ENUM, BIT_SHIFT, FORMAT, ...)                                \
    case GSHL_LOG_##ENUM: {                                                    \
        count += GSHL_format_write(string, FORMAT, #ENUM);                     \
    } break;
            GSHL_LOG_VERBOSITY_LEVELS
#undef VERBOSITY
        default:
            break;
        }
    }

    return count;
}

usize write_struct_LogConfig(struct GSHL_FormatString *string,
                             const struct GSHL_FormatSpecifier *fs)
{
    GSHL_ASSERT(fs != NULL);
    const GSHL_LogConfig *config = fs->value.pointer;

    return GSHL_format_write(string,
                             "struct LogConfig {{\n"
                             "  .fd = %i\n"
                             "  .mask = {enum GSHL_LogKind}\n"
                             "  .file_fd = %i\n"
                             "  .print = {{\n"
                             "    .function = %u\n"
                             "    .file = %B\n"
                             "  }}\n"
                             "}}",
                             config->fd, config->mask, config->file_fd,
                             (u32)config->print.function, config->print.file);
}

void GSHL_log_init_wrapper(const GSHL_LogConfig config)
{
    static const GSHL_LogConfig null_config = {0};
    if (config.fd != null_config.fd) {
        if (config.fd == -1) {
            perror("log_config.fd");
            return;
        }

        log_config.fd = config.fd;
    }

    if (config.mask != null_config.mask) {
        log_config.mask = config.mask;
        GSHL_log_read_env();
    }

    if (config.file_fd != null_config.file_fd) {
        if (config.file_fd == -1) {
            perror("log_config.file_fd");
            return;
        }

        log_config.file_fd = config.file_fd;
    }

    if (log_config.print.file != config.print.file) {
        log_config.print.file = config.print.file;
    }

    if (log_config.print.function != config.print.function) {
        log_config.print.function = config.print.function;
    }

    GSHL_ASSERT(log_config.fd > 0);

    GSHL_format_specifier_register((GSHL_FormatSpecifier){
        .kind = GSHL_FORMAT_SPECIFIER_U32,
        .va_size = sizeof(GSHL_LogKind),
        .write = write_enum_LogKind,
        .specifiers = {"enum GSHL_LogKind", "GSHL_LogKind", "enum LogKind",
                       "LogKind"},
    });

    GSHL_format_specifier_register((GSHL_FormatSpecifier){
        .kind = GSHL_FORMAT_SPECIFIER_POINTER,
        .va_size = sizeof(GSHL_LogConfig *),
        .write = write_struct_LogConfig,
        .specifiers = {"struct GSHL_LogConfig", "GSHL_LogConfig",
                       "struct LogConfig", "LogConfig"},
    });
}

GSHL_LogConfig GSHL_log_get_config(void) { return log_config; }

bool GSHL_log_is_allowed(const GSHL_LogKind kind)
{
    GSHL_ASSERT(GSHL_is_power_of_two(kind) || kind == GSHL_LOG_NONE);

    return (kind & log_config.mask) != GSHL_LOG_NONE;
}

usize GSHL_log_wrapper(const GSHL_LogKind kind, const GSHL_LogOpts opts,
                       const char *const restrict format, ...)
{
    if (!GSHL_log_is_allowed(kind)) {
        return 0;
    }

    if (log_config.fd <= 0) {
        GSHL_dprintln(STDERR_FILENO, "log_config.fd = %i", log_config.fd);
        GSHL_dprintln(STDERR_FILENO, "log_config.file_fd = %i",
                      log_config.file_fd);
        return 0;
    }

    GSHL_FormatString string = {};
    GSHL_DArray_init(&string);

    static const char iden[] = "  ";
    static const usize idenLen = GSHL_STACK_STRING_LEN(iden);
    GSHL_format_write(&string, "%s[{enum GSHL_LogKind}]: ", iden, kind);

    if (log_config.print.file) {
        GSHL_format_write(&string, "%s:%lu > ", opts.file, opts.line);
    }

    switch (log_config.print.function) {
    case GSHL_LOG_PRINT_FUNCTION:
        GSHL_format_write(&string, "%s > ", opts.function);
        break;
    case GSHL_LOG_PRINT_PRETTY_FUNCTION:
        GSHL_format_write(&string, "%s > ", opts.pretty_function);
        break;
    default:
        GSHL_UNREACHABLE("Unknown log_config.print.function = %i",
                         log_config.print.function);
        break;
    }

    va_list args;
    va_start(args, format);
    GSHL_format_writev(&string, format, args);
    va_end(args);

    GSHL_DArray_append(&string, '\n');
    GSHL_DArray_append(&string, '\0');

    usize count = 0;
    if (log_config.file_fd > 0) {
        count = write(log_config.file_fd, string.items + idenLen,
                      string.len - idenLen - 1);
    }

    if (log_config.fd > 0) {
        count = write(log_config.fd, string.items, string.len - 1);
    }

    GSHL_ASSERT(count > 0);

    GSHL_DArray_destroy(&string);

    return count;
}

void GSHL_log_read_env(void)
{
    static const struct GSHL_Env2Log {
        char *values[2];
        GSHL_LogKind kind;
    } conversions[] = {
#define VERBOSITY(ENUM, BIT_SHIFT, FORMAT, ...)                                \
    {.values = {__VA_ARGS__}, .kind = GSHL_LOG_##ENUM},
        GSHL_LOG_VERBOSITY_LEVELS
#undef VERBOSITY

    };

    const char *const verbosity = getenv("GSHL_LOG");
    if (verbosity == NULL) {
        return;
    }

    log_config.mask = 0;
    GSHL_ARRAY_FOREACH(conversions, struct GSHL_Env2Log env2log)
    {
        GSHL_ARRAY_FOREACH(env2log.values, char *value)
        {
            if (strstr(verbosity, value) == NULL) {
                continue;
            }

            log_config.mask |= env2log.kind;
        }
    }
}

void GSHL_log_print_config(void) { GSHL_println("{LogConfig}", &log_config); }
#ifdef GSHL_SOURCE_CODE_MAPPING
#    line 0 "src/hash_table/mod.c"
#endif // GSHL_SOURCE_CODE_MAPPING
// #include "hash_table/mod.h"
// #include "macros/mod.h"

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
// #    include "test/mod.h"

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
#ifdef GSHL_SOURCE_CODE_MAPPING
#    line 0 "src/hash_set/mod.c"
#endif // GSHL_SOURCE_CODE_MAPPING
#ifdef GSHL_SOURCE_CODE_MAPPING
#    line 0 "src/hash/kr.c"
#endif // GSHL_SOURCE_CODE_MAPPING
// #include "hash/kr.h"

usize GSHL_hash_kr(const char *const string)
{
    usize h = 0;

    for (unsigned char *c = (unsigned char *)string; *c != '\0'; c++) {
        h += *c + (h << 5) - h;
    }

    return h;
}
#ifdef GSHL_SOURCE_CODE_MAPPING
#    line 0 "src/hash/fnv1a.c"
#endif // GSHL_SOURCE_CODE_MAPPING
// #include "hash/fnv1a.h"

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
// #    include "test/mod.h"

GSHL_TEST(hash_fnv1a32)
{
    GSHL_TEST_EQUAL(GSHL_hash_fnv1a32("example"), 0x8BF23EA1);
    GSHL_TEST_EQUAL(GSHL_hash_fnv1a32("test"), 0xAFD071E5);
    GSHL_TEST_EQUAL(GSHL_hash_fnv1a32("hash"), 0xCEC577D1);
}

#endif
#ifdef GSHL_SOURCE_CODE_MAPPING
#    line 0 "src/hash/djb2.c"
#endif // GSHL_SOURCE_CODE_MAPPING
// #include "hash/djb2.h"

usize GSHL_hash_djb2(const char *string)
{
    usize hash = 5381;

    for (unsigned char *c = (unsigned char *)string; *c != '\0'; ++c) {
        hash = (hash << 5) + hash + *c; // hash * 33 + c
    }

    return hash;
}
#ifdef GSHL_SOURCE_CODE_MAPPING
#    line 0 "src/format/usize.c"
#endif // GSHL_SOURCE_CODE_MAPPING
// #include "format/usize.h"

// #include "format/u16.h"
// #include "format/u32.h"
// #include "format/u64.h"
// #include "format/u8.h"
// #include "macros/mod.h"
// #include "string/mod.h"

#include <assert.h>
#include <stdlib.h>

usize GSHL_write_usize(GSHL_FormatString *string,
                       const GSHL_FormatSpecifier *fs)
{
    GSHL_ASSERT(fs->kind &
                (GSHL_FORMAT_SPECIFIER_U8 | GSHL_FORMAT_SPECIFIER_U16 |
                 GSHL_FORMAT_SPECIFIER_U32 | GSHL_FORMAT_SPECIFIER_U64 |
                 GSHL_FORMAT_SPECIFIER_USIZE));
    GSHL_ASSERT(fs->write == GSHL_write_u8 || fs->write == GSHL_write_u16 ||
                fs->write == GSHL_write_u32 || fs->write == GSHL_write_u64 ||
                fs->write == GSHL_write_usize);

    usize rem = 0;
    switch (fs->kind) {
    case GSHL_FORMAT_SPECIFIER_U8:
        rem = fs->value.u8;
        break;
    case GSHL_FORMAT_SPECIFIER_U16:
        rem = fs->value.u16;
        break;
    case GSHL_FORMAT_SPECIFIER_U32:
        rem = fs->value.u32;
        break;
    case GSHL_FORMAT_SPECIFIER_U64:
        rem = fs->value.u64;
        break;
    case GSHL_FORMAT_SPECIFIER_USIZE:
        rem = fs->value.usize;
        break;
    default:
        GSHL_UNREACHABLE("Invalid kind %i", fs->kind);
        break;
    }

    usize count = 0;
    do {
        if (string != NULL) {
            GSHL_DArray_append(string, '0' + (rem % 10));
        }

        rem /= 10;
        count += 1;
    } while (rem > 0);

    if (string != NULL) {
        GSHL_string_reverse(&string->items[string->count - count],
                            &string->items[string->count]);
    }

    return count;
}

#ifdef GSHL_TESTS
// #    include "test/mod.h"

GSHL_TEST(write_usize)
{

#    define TEST_WRITE_usize(EXPRESSION, EXPECTED)                             \
        do {                                                                   \
            GSHL_FormatString string = {};                                     \
            GSHL_DArray_init(&string);                                         \
            const GSHL_FormatSpecifier fs = {                                  \
                .kind = GSHL_FORMAT_SPECIFIER_USIZE,                           \
                .write = GSHL_write_usize,                                     \
                .value.usize = (EXPRESSION),                                   \
            };                                                                 \
                                                                               \
            const usize count = GSHL_write_usize(&string, &fs);                \
            GSHL_DArray_append(&string, '\0');                                 \
            GSHL_TEST_EQUAL(count, GSHL_STACK_STRING_LEN(EXPECTED));           \
            GSHL_TEST_STR_EQUAL(string.items, EXPECTED);                       \
            GSHL_DArray_destroy(&string);                                      \
        } while (0)

    TEST_WRITE_usize(42, "42");
    TEST_WRITE_usize(0, "0");
    TEST_WRITE_usize(294482, "294482");
    TEST_WRITE_usize(-294482U, "4294672814");
    TEST_WRITE_usize(~0U, "4294967295");
    TEST_WRITE_usize(0xFF, "255");

#    undef TEST_WRITE_usize
}

#endif
#ifdef GSHL_SOURCE_CODE_MAPPING
#    line 0 "src/format/u8.c"
#endif // GSHL_SOURCE_CODE_MAPPING
// #include "format/u8.h"

// #include "format/usize.h"
// #include "macros/mod.h"
// #include "string/mod.h"

#include <assert.h>
#include <stdlib.h>

usize GSHL_write_u8(GSHL_FormatString *string, const GSHL_FormatSpecifier *fs)
{
    GSHL_ASSERT(fs->kind == GSHL_FORMAT_SPECIFIER_U8);
    GSHL_ASSERT(fs->write == GSHL_write_u8);

    return GSHL_write_usize(string, fs);
}

#ifdef GSHL_TESTS
// #    include "test/mod.h"

GSHL_TEST(write_u8)
{

#    define TEST_WRITE_u8(EXPRESSION, EXPECTED)                                \
        do {                                                                   \
            GSHL_FormatString string = {};                                     \
            GSHL_DArray_init(&string);                                         \
            const GSHL_FormatSpecifier fs = {                                  \
                .kind = GSHL_FORMAT_SPECIFIER_U8,                              \
                .write = GSHL_write_u8,                                        \
                .value.u8 = (EXPRESSION),                                      \
            };                                                                 \
                                                                               \
            const u8 count = GSHL_write_u8(&string, &fs);                      \
            GSHL_DArray_append(&string, '\0');                                 \
            GSHL_TEST_EQUAL(count, GSHL_STACK_STRING_LEN(EXPECTED));           \
            GSHL_TEST_STR_EQUAL(string.items, EXPECTED);                       \
            GSHL_DArray_destroy(&string);                                      \
        } while (0)

    TEST_WRITE_u8(42, "42");
    TEST_WRITE_u8(0, "0");
    TEST_WRITE_u8(0xFF, "255");

#    undef TEST_WRITE_u8
}

#endif
#ifdef GSHL_SOURCE_CODE_MAPPING
#    line 0 "src/format/u64.c"
#endif // GSHL_SOURCE_CODE_MAPPING
// #include "format/u64.h"

// #include "format/usize.h"
// #include "macros/mod.h"
// #include "string/mod.h"

#include <assert.h>
#include <stdlib.h>

usize GSHL_write_u64(GSHL_FormatString *string, const GSHL_FormatSpecifier *fs)
{
    GSHL_ASSERT(fs->kind == GSHL_FORMAT_SPECIFIER_U64);
    GSHL_ASSERT(fs->write == GSHL_write_u64);

    return GSHL_write_usize(string, fs);
}

#ifdef GSHL_TESTS
// #    include "test/mod.h"

GSHL_TEST(write_u64)
{

#    define TEST_WRITE_u64(EXPRESSION, EXPECTED)                               \
        do {                                                                   \
            GSHL_FormatString string = {};                                     \
            GSHL_DArray_init(&string);                                         \
            const GSHL_FormatSpecifier fs = {                                  \
                .kind = GSHL_FORMAT_SPECIFIER_U64,                             \
                .write = GSHL_write_u64,                                       \
                .value.u64 = (EXPRESSION),                                     \
            };                                                                 \
                                                                               \
            const u64 count = GSHL_write_u64(&string, &fs);                    \
            GSHL_DArray_append(&string, '\0');                                 \
            GSHL_TEST_EQUAL(count, GSHL_STACK_STRING_LEN(EXPECTED));           \
            GSHL_TEST_STR_EQUAL(string.items, EXPECTED);                       \
            GSHL_DArray_destroy(&string);                                      \
        } while (0)

    TEST_WRITE_u64(42, "42");
    TEST_WRITE_u64(0, "0");
    TEST_WRITE_u64(294482, "294482");
    TEST_WRITE_u64(-294482U, "4294672814");
    TEST_WRITE_u64(~0U, "4294967295");
    TEST_WRITE_u64(0xFF, "255");

#    undef TEST_WRITE_u64
}

#endif
#ifdef GSHL_SOURCE_CODE_MAPPING
#    line 0 "src/format/u32.c"
#endif // GSHL_SOURCE_CODE_MAPPING
// #include "format/u32.h"

// #include "format/usize.h"
// #include "macros/mod.h"
// #include "string/mod.h"

#include <assert.h>
#include <stdlib.h>

usize GSHL_write_u32(GSHL_FormatString *string, const GSHL_FormatSpecifier *fs)
{
    GSHL_ASSERT(fs->kind == GSHL_FORMAT_SPECIFIER_U32);
    GSHL_ASSERT(fs->write == GSHL_write_u32);

    return GSHL_write_usize(string, fs);
}

#ifdef GSHL_TESTS
// #    include "test/mod.h"

GSHL_TEST(write_u32)
{

#    define TEST_WRITE_u32(EXPRESSION, EXPECTED)                               \
        do {                                                                   \
            GSHL_FormatString string = {};                                     \
            GSHL_DArray_init(&string);                                         \
            const GSHL_FormatSpecifier fs = {                                  \
                .kind = GSHL_FORMAT_SPECIFIER_U32,                             \
                .write = GSHL_write_u32,                                       \
                .value.u32 = (EXPRESSION),                                     \
            };                                                                 \
                                                                               \
            const usize count = GSHL_write_u32(&string, &fs);                  \
            GSHL_DArray_append(&string, '\0');                                 \
            GSHL_TEST_EQUAL(count, GSHL_STACK_STRING_LEN(EXPECTED));           \
            GSHL_TEST_STR_EQUAL(string.items, EXPECTED);                       \
            GSHL_DArray_destroy(&string);                                      \
        } while (0)

    TEST_WRITE_u32(42, "42");
    TEST_WRITE_u32(0, "0");
    TEST_WRITE_u32(294482, "294482");
    TEST_WRITE_u32(-294482U, "4294672814");
    TEST_WRITE_u32(~0U, "4294967295");
    TEST_WRITE_u32(0xFF, "255");

#    undef TEST_WRITE_u32
}

#endif
#ifdef GSHL_SOURCE_CODE_MAPPING
#    line 0 "src/format/u16.c"
#endif // GSHL_SOURCE_CODE_MAPPING
// #include "format/u16.h"

// #include "format/usize.h"
// #include "macros/mod.h"
// #include "string/mod.h"

#include <assert.h>
#include <stdlib.h>

usize GSHL_write_u16(GSHL_FormatString *string, const GSHL_FormatSpecifier *fs)
{
    GSHL_ASSERT(fs->kind == GSHL_FORMAT_SPECIFIER_U16);
    GSHL_ASSERT(fs->write == GSHL_write_u16);

    return GSHL_write_usize(string, fs);
}

#ifdef GSHL_TESTS
// #    include "test/mod.h"

GSHL_TEST(write_u16)
{

#    define TEST_WRITE_u16(EXPRESSION, EXPECTED)                               \
        do {                                                                   \
            GSHL_FormatString string = {};                                     \
            GSHL_DArray_init(&string);                                         \
            const GSHL_FormatSpecifier fs = {                                  \
                .kind = GSHL_FORMAT_SPECIFIER_U16,                             \
                .write = GSHL_write_u16,                                       \
                .value.u16 = (EXPRESSION),                                     \
            };                                                                 \
                                                                               \
            const u16 count = GSHL_write_u16(&string, &fs);                    \
            GSHL_DArray_append(&string, '\0');                                 \
            GSHL_TEST_EQUAL(count, GSHL_STACK_STRING_LEN(EXPECTED));           \
            GSHL_TEST_STR_EQUAL(string.items, EXPECTED);                       \
            GSHL_DArray_destroy(&string);                                      \
        } while (0)

    TEST_WRITE_u16(42, "42");
    TEST_WRITE_u16(0, "0");
    TEST_WRITE_u16(2944, "2944");
    TEST_WRITE_u16(0xFF, "255");

#    undef TEST_WRITE_u16
}

#endif
#ifdef GSHL_SOURCE_CODE_MAPPING
#    line 0 "src/format/pointer.c"
#endif // GSHL_SOURCE_CODE_MAPPING
// #include "format/pointer.h"

// #include "format/hex.h"
// #include "macros/mod.h"
// #include "string/mod.h"
// #include "types/mod.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

usize GSHL_write_pointer(GSHL_FormatString *string,
                         const GSHL_FormatSpecifier *fs)
{
    GSHL_ASSERT(fs->kind == GSHL_FORMAT_SPECIFIER_POINTER);
    GSHL_ASSERT(fs->write == GSHL_write_pointer);

    const usize start_count = string->count;

    const pointer pointer = fs->value.pointer;
    if (pointer == NULL) {
        GSHL_DArray_extendn(string, "nil", GSHL_STACK_STRING_LEN("nil"));
        return string->count - start_count;
    }

    GSHL_DArray_extendn(string, "0x", GSHL_STACK_STRING_LEN("0x"));
    GSHL_write_hex64(string, fs);

    return string->count - start_count;
}

#ifdef GSHL_TESTS
// #    include "test/mod.h"

GSHL_TEST(write_pointer)
{

#    define TEST_WRITE_pointer(EXPRESSION, EXPECTED, ...)                      \
        do {                                                                   \
            GSHL_FormatString string = {};                                     \
            GSHL_DArray_init(&string);                                         \
            const GSHL_FormatSpecifier fs = {                                  \
                .kind = GSHL_FORMAT_SPECIFIER_POINTER,                         \
                .write = GSHL_write_pointer,                                   \
                .value.pointer = (EXPRESSION),                                 \
                .opts.pointer = {__VA_ARGS__},                                 \
            };                                                                 \
                                                                               \
            const usize count = GSHL_write_pointer(&string, &fs);              \
            const char expected[] = EXPECTED;                                  \
            GSHL_DArray_append(&string, '\0');                                 \
            GSHL_TEST_EQUAL(count, GSHL_STACK_STRING_LEN(EXPECTED));           \
            GSHL_TEST_STR_EQUAL(string.items, expected);                       \
            GSHL_DArray_destroy(&string);                                      \
        } while (0)

    TEST_WRITE_pointer(NULL, "nil");
    TEST_WRITE_pointer((void *)0x7ffef80057a4, "0x7ffef80057a4");
    TEST_WRITE_pointer((void *)0x7ffef80057a4, "0x7FFEF80057A4",
                       .uppercase = true);

#    undef TEST_WRITE_pointer
}

#endif
#ifdef GSHL_SOURCE_CODE_MAPPING
#    line 0 "src/format/mod.c"
#endif // GSHL_SOURCE_CODE_MAPPING
// #include "format/mod.h"

// #include "format/bool.h"    // IWYU pragma: keep
// #include "format/char.h"    // IWYU pragma: keep
// #include "format/cstring.h" // IWYU pragma: keep
// #include "format/hex.h"     // IWYU pragma: keep
// #include "format/i16.h"     // IWYU pragma: keep
// #include "format/i32.h"     // IWYU pragma: keep
// #include "format/i64.h"     // IWYU pragma: keep
// #include "format/i8.h"      // IWYU pragma: keep
// #include "format/isize.h"   // IWYU pragma: keep
// #include "format/pointer.h" // IWYU pragma: keep
// #include "format/u16.h"     // IWYU pragma: keep
// #include "format/u32.h"     // IWYU pragma: keep
// #include "format/u64.h"     // IWYU pragma: keep
// #include "format/u8.h"      // IWYU pragma: keep
// #include "format/usize.h"   // IWYU pragma: keep

// #include "ansi/mod.h"
// #include "array/mod.h"
// #include "hash_table/mod.h"
// #include "macros/mod.h"
// #include "print/mod.h"
// #include "string/mod.h"

#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static __thread GSHL_HashTable GSHL_format_ht = {};

static usize GSHL_hash_format_specifier(const char *const start,
                                        const char *const end,
                                        const char **startP)
{
    u32 hash = 2166136261u; // FNV offset basis

    GSHL_ASSERT(start != NULL);

    if (startP == NULL) {
        GSHL_ASSERT(start <= end);

        // Reasonable length for a format specifier
        GSHL_ASSERT(end - start < GSHL_FORMAT_SPECIFIER_MAX_LEN &&
                    ("Exceeded max format specifier length"));

        for (const char *c = start; c < end; c += 1) {
            hash ^= *(unsigned char *)c;
            hash *= 16777619; // FNV prime
        }
    }
    else {
        GSHL_ASSERT(end == NULL);

        // Point to the first character of the format specifier
        const char *formatStart = *startP;
        // Point to one after the last character of the format specifier
        const char *formatEnd = *startP + 1;

        const char *specifierStart;
        const char *specifierEnd;

        switch (*formatStart) {
        case '%': {
            formatEnd = formatStart + (formatStart[1] == 'l') + 2;
            *startP = formatEnd;
            return GSHL_hash_format_specifier(formatStart, formatEnd, NULL);
        } break;
        case '{':
            specifierStart = formatStart + 1;
            specifierEnd = specifierStart;

            for (bool exit = false; !exit && *formatEnd != '\0';) {
                switch (*formatEnd) {
                case '}':
                case ';':
                    exit = true;
                    break;
                default:
                    formatEnd += 1;
                    break;
                }
            }

            specifierEnd = formatEnd;

            if (formatEnd[0] != '}') {
                fprintf(stderr, "Unknown format specifier \"%.*s\" \"%.*s\"\n",
                        (int)(formatEnd - formatStart), formatStart,
                        (int)(specifierEnd - specifierStart), specifierStart);
                return 0;
            }

            formatEnd += 1;
            *startP = formatEnd;

            return GSHL_hash_format_specifier(specifierStart, specifierEnd,
                                              NULL);
        default:
            return 0;
        }
    }

    return hash;
}

static usize GSHL_hash_idem(const GSHL_HashTableKey key) { return key.usize; }

__attribute__((constructor)) static void GSHL_init_ht()
{
    GSHL_FormatSpecifier format_specifiers[] = {
#define FS(BIT_SHIFT, ENUM, NAME, TYPE, OPTS, ...)                             \
    [BIT_SHIFT] = {                                                            \
        .kind = GSHL_FORMAT_SPECIFIER_##ENUM,                                  \
        .write = GSHL_write_##TYPE,                                            \
        .va_size = (((sizeof(TYPE) - 1) / 4) + 1) * 4,                         \
        .specifiers = {__VA_ARGS__},                                           \
    },
        GSHL_FORMAT_SPECIFIERS
#undef FS
    };

    GSHL_HashTable_init(&GSHL_format_ht, usize, GSHL_FormatSpecifier *,
                        GSHL_hash_idem);

    GSHL_ARRAY_FOREACH(format_specifiers, GSHL_FormatSpecifier fs)
    {
        if (fs.write == NULL) {
            continue;
        }

        GSHL_ASSERT(fs.va_size == 4 || fs.va_size == 8);

#if 0
        printf("FormatSpecifier {\n"
               "  .kind = %i\n"
               "  .va_size = %lu\n"
               "  .type_string = %s\n"
               "}\n",
               fs.kind, fs.va_size, fs.type_string);
#endif

        GSHL_format_specifier_register(fs);
    }

#if 0
    GSHL_HashTable_print(
        GSHL_format_ht, "%lu", usize, "%p", opaque,
        "FormatSpecifier {\n"
        "  .kind = %u\n"
        "  .write = %p\n"
        "  .va_size = %lu\n"
        "  .type_string = %s\n"
        "}",
        ((GSHL_FormatSpecifier *)current->value.opaque)->kind,
        ((GSHL_FormatSpecifier *)current->value.opaque)->write,
        ((GSHL_FormatSpecifier *)current->value.opaque)->va_size,
        ((GSHL_FormatSpecifier *)current->value.opaque)->type_string);
#endif
}

GSHL_StringView GSHL_format_wrapper(const char *restrict format, ...)
{
    va_list args;
    va_start(args, format);
    const GSHL_StringView view = GSHL_format_wrapperv(format, args);
    va_end(args);

    return view;
}

GSHL_StringView GSHL_format_wrapperv(const char *const restrict format,
                                     va_list args)
{
    GSHL_ASSERT(format != NULL);

    static __thread GSHL_FormatString strings[10] = {};
    static __thread usize strings_index = 0;

    GSHL_FormatString *string = &strings[strings_index];
    strings_index = (strings_index + 1) % GSHL_ARRAY_LEN(strings);

    GSHL_DArray_init(string);

    GSHL_format_writev(string, format, args);

    GSHL_DArray_append(string, '\0');

    return (GSHL_StringView){
        .start = string->items,
        .len = string->count - 1,
    };
}

usize GSHL_format_write(GSHL_FormatString *string,
                        const char *const restrict format, ...)
{
    va_list args;
    va_start(args, format);
    const usize count = GSHL_format_writev(string, format, args);
    va_end(args);
    return count;
}

usize GSHL_format_writev(GSHL_FormatString *string,
                         const char *const restrict format, va_list args)
{
    const char *formatEnd = format;

    for (const char *formatC = format; *formatC != '\0'; formatC += 1) {
        formatEnd = formatC;

        if ((formatC[0] == '{' && formatC[1] == '{') ||
            (formatC[0] == '}' && formatC[1] == '}')) {
            GSHL_DArray_append(string, formatC[1]);
            formatC += 1;
            continue;
        }

        const usize fs_hash =
            GSHL_hash_format_specifier(formatC, NULL, &formatEnd);

        if (formatC == formatEnd) {
            GSHL_DArray_append(string, *formatC);
        }
        else {
            const GSHL_HashTableValue *valueP =
                GSHL_HashTable_search(&GSHL_format_ht, .usize = fs_hash);
            if (valueP == NULL) {
                fprintf(stderr, "format: \"%s\" \"%.*s\"\n", format,
                        (int)(formatEnd - formatC), formatC);
                fprintf(stderr, "hash: %lu\n", fs_hash);
                fprintf(stderr, "valueP: %p\n", valueP);
                fprintf(stderr, "Unknown format specifier '%.*s'\n",
                        (int)(formatEnd - formatC), formatC);

                for (; formatC < formatEnd; formatC += 1) {
                    GSHL_DArray_append(string, *formatC);
                }

                formatC = formatEnd - 1;
                continue;
            }

            GSHL_FormatSpecifier *const fs = valueP->opaque;
            GSHL_ASSERT(fs != NULL);

            switch (fs->va_size) {
            case 4:
                fs->value.u32 = va_arg(args, u32);
                break;
            case 8:
                fs->value.u64 = va_arg(args, u64);
                break;
            default:
                GSHL_UNREACHABLE("Wrong fs->va_size = %lu", fs->va_size);
                break;
            }

            fs->write(string, fs);

            formatC = formatEnd - 1;
        }
    }

    GSHL_ASSERT(format <= formatEnd);

    return (usize)(formatEnd - format);
}

char *GSHL_format(const char *const restrict format, ...)
{
    va_list args;
    va_start(args, format);
    char *string = GSHL_formatv(format, args);
    va_end(args);

    return string;
}

char *GSHL_formatv(const char *const restrict format, va_list args)
{
    const GSHL_StringView view = GSHL_format_wrapperv(format, args);

    return GSHL_string_dup(view.start);
}

void GSHL_print_registered_format_specifiers(void)
{
#if 0
    GSHL_HashTable_print(
        GSHL_format_ht, "%lu", usize, "%p", opaque,
        "FormatSpecifier {\n"
        "    .kind = %u\n"
        "    .write = %p\n"
        "    .va_size = %lu\n"
        "    .type_string = %s\n"
        "  }",
        ((GSHL_FormatSpecifier *)current->value.opaque)->kind,
        ((GSHL_FormatSpecifier *)current->value.opaque)->write,
        ((GSHL_FormatSpecifier *)current->value.opaque)->va_size,
        ((GSHL_FormatSpecifier *)current->value.opaque)->type_string);
#else
    GSHL_ARRAYN_FOREACH(GSHL_format_ht.table, GSHL_format_ht.table_size,
                        GSHL_HashTableEntry * entry)
    {
        for (const GSHL_HashTableEntry *current = entry; current != NULL;
             current = current->next) {
            const GSHL_FormatSpecifier *fs = current->value.opaque;
            GSHL_ARRAY_FOREACH(fs->specifiers, char *specifier)
            {
                if (specifier == NULL || specifier[0] == '\0') {
                    break;
                }

                printf(GSHL_FG_CYAN("%s") "\n", specifier);
            }
        }
    }
#endif
}

bool GSHL_format_specifier_register(const GSHL_FormatSpecifier fs)
{
    static __thread GSHL_FormatSpecifiers format_specifiers = {};

    GSHL_DArray_append(&format_specifiers, fs);

    GSHL_ARRAY_FOREACH(fs.specifiers, char *specifier)
    {
        if (specifier == NULL || specifier[0] == '\0') {
            break;
        }

        const char *const start = specifier;
        const char *const end = specifier + strlen(specifier);
        const usize hash = GSHL_hash_format_specifier(start, end, NULL);
        if (GSHL_HashTable_search(&GSHL_format_ht, .usize = hash) != NULL) {
#ifdef GSHL_DEBUG
            GSHL_dprintln(STDERR_FILENO,
                          "Format Specifier already registered: %s", specifier);
#endif
            continue;
        }

        GSHL_ASSERT(fs.va_size % 4 == 0);

        GSHL_HashTable_insert(
            &GSHL_format_ht, .usize = hash,
            .opaque = &format_specifiers.items[format_specifiers.count - 1]);
    }

    return true;
}

#ifdef GSHL_TESTS
// #    include "test/mod.h"

GSHL_TEST(format_wrapper)
{
#    define TEST_FORMAT_WRAPPER(EXPECTED_STRING, ...)                          \
        do {                                                                   \
            const char output[] = EXPECTED_STRING;                             \
            const usize expected_count = GSHL_STACK_STRING_LEN(output);        \
            const GSHL_StringView view = GSHL_format_wrapper(__VA_ARGS__);     \
            GSHL_TEST_EQUAL(view.len, expected_count);                         \
            GSHL_TEST_STR_EQUAL(view.start, output);                           \
        } while (0)

    TEST_FORMAT_WRAPPER("", "");
    TEST_FORMAT_WRAPPER("Hello world!", "Hello world!");
    TEST_FORMAT_WRAPPER("34 + 35 = 69", "%u + %u = %u", 34, 35, 69);
    TEST_FORMAT_WRAPPER("34 + 35 = 69", "{u32} + {u32} = {u32}", 34, 35, 69);

    TEST_FORMAT_WRAPPER("Foo bar baz", "%s %s %s", "Foo", "bar", "baz");
    TEST_FORMAT_WRAPPER("Foo bar baz", "{cstring} {cstring} {cstring}", "Foo",
                        "bar", "baz");

    // Test integer formatting
    TEST_FORMAT_WRAPPER("1 = 1", "%d = %d", 1, 1);
    TEST_FORMAT_WRAPPER("10 + 20 = 30", "%d + %d = %d", 10, 20, 30);
    TEST_FORMAT_WRAPPER("100 - 50 = 50", "%d - %d = %d", 100, 50, 50);
    TEST_FORMAT_WRAPPER("1 = 1", "{i32} = {i32}", 1, 1);
    TEST_FORMAT_WRAPPER("10 + 20 = 30", "{i32} + {i32} = {i32}", 10, 20, 30);
    TEST_FORMAT_WRAPPER("100 - 50 = 50", "{i32} - {i32} = {i32}", 100, 50, 50);

    // Test unsigned integer formatting
    TEST_FORMAT_WRAPPER("42 = 42", "%u = %u", 42, 42);
    TEST_FORMAT_WRAPPER("8 + 2 = 10", "%u + %u = %u", 8, 2, 10);
    TEST_FORMAT_WRAPPER("42 = 42", "{u32} = {u32}", 42, 42);
    TEST_FORMAT_WRAPPER("8 + 2 = 10", "{u32} + {u32} = {u32}", 8, 2, 10);

    // Test floating point formatting
    // TEST_FORMAT_WRAPPER("Pi ~ 3.14", "Pi is approximately %.2f", 3.14159);
    // TEST_FORMAT_WRAPPER("The number is 2.71828", "The number is
    // %f", 2.71828); TEST_FORMAT_WRAPPER("Gravity = 9.81 m/s^2", "Gravity =
    // %.2f m/s^2", 9.81);

    // Test character formatting
    TEST_FORMAT_WRAPPER("The first letter of the alphabet is A",
                        "The first letter of the alphabet is {character}", 'A');
    TEST_FORMAT_WRAPPER(
        "Coding",
        "{character}{character}{character}{character}{character}{character}",
        'C', 'o', 'd', 'i', 'n', 'g');

    // Test string formatting
    TEST_FORMAT_WRAPPER("Hello, World!", "%s", "Hello, World!");
    TEST_FORMAT_WRAPPER("Name: Alice", "Name: %s", "Alice");

    // Test hexadecimal formatting
    // TEST_FORMAT_WRAPPER("Hex 255 = FF", "Hex %d = %X", 255, 255);
    TEST_FORMAT_WRAPPER("Hex 16 = 10", "Hex %d = %x", 16, 16);

    // Test octal formatting
    // TEST_FORMAT_WRAPPER("Octal 8 = 10", "Octal %d = %o", 8, 8);
    // TEST_FORMAT_WRAPPER("Octal 64 = 100", "Octal %d = %o", 64, 64);

    // Test pointer formatting (simulated with address output)
    TEST_FORMAT_WRAPPER("Address of variable: 0x7ffef80057a4",
                        "Address of variable: %p", (void *)0x7ffef80057a4);

    // Test printing size_t
    usize size = 100;
    TEST_FORMAT_WRAPPER("Size = 100", "Size = %lu", size);

    TEST_FORMAT_WRAPPER("foo bar", "%s {string}", "foo", "bar");
    TEST_FORMAT_WRAPPER("34 35", "%u {u32}", 34, 35);
    TEST_FORMAT_WRAPPER("34 35 69", "%lu {u64} {usize}", 34L, 35L, 69L);
    TEST_FORMAT_WRAPPER("-34 -35 -69", "%i %d {i32}", -34, -35, -69);
    TEST_FORMAT_WRAPPER("-34 -35 -69", "%li %ld {isize}", -34L, -35L, -69L);
    TEST_FORMAT_WRAPPER("A B C", "%c {character} {char}", 'A', 'B', 'C');

#    undef TEST_FORMAT_WRAPPER
}

#    undef GSHL_FORMAT_SPECIFIER_ASSERT

#endif
#ifdef GSHL_SOURCE_CODE_MAPPING
#    line 0 "src/format/isize.c"
#endif // GSHL_SOURCE_CODE_MAPPING
// #include "format/isize.h"

// #include "format/i16.h"
// #include "format/i32.h"
// #include "format/i64.h"
// #include "format/i8.h"
// #include "macros/mod.h"
// #include "string/mod.h"

#include <assert.h>
#include <stdlib.h>

usize GSHL_write_isize(GSHL_FormatString *string,
                       const GSHL_FormatSpecifier *fs)
{
    GSHL_ASSERT(fs->kind &
                (GSHL_FORMAT_SPECIFIER_I8 | GSHL_FORMAT_SPECIFIER_I16 |
                 GSHL_FORMAT_SPECIFIER_I32 | GSHL_FORMAT_SPECIFIER_I64 |
                 GSHL_FORMAT_SPECIFIER_ISIZE));
    GSHL_ASSERT(fs->write == GSHL_write_i8 || fs->write == GSHL_write_i16 ||
                fs->write == GSHL_write_i32 || fs->write == GSHL_write_i64 ||
                fs->write == GSHL_write_isize);

    isize rem = 0;
    switch (fs->kind) {
    case GSHL_FORMAT_SPECIFIER_I8:
        rem = fs->value.i8;
        break;
    case GSHL_FORMAT_SPECIFIER_I16:
        rem = fs->value.i16;
        break;
    case GSHL_FORMAT_SPECIFIER_I32:
        rem = fs->value.i32;
        break;
    case GSHL_FORMAT_SPECIFIER_I64:
        rem = fs->value.i64;
        break;
    case GSHL_FORMAT_SPECIFIER_ISIZE:
        rem = fs->value.isize;
        break;
    default:
        GSHL_UNREACHABLE("Invalid kind %i", fs->kind);
        break;
    }

    bool neg = rem < 0;

    usize count = 0;

    if (neg) {
        rem *= -1;
        if (string != NULL) {
            GSHL_DArray_append(string, '-');
        }
        count += 1;
    }

    do {
        if (string != NULL) {
            GSHL_DArray_append(string, '0' + (rem % 10));
        }

        rem /= 10;
        count += 1;
    } while (rem > 0);

    if (string != NULL) {
        GSHL_string_reverse(&string->items[string->count - count + !!neg],
                            &string->items[string->count]);
    }

    return count;
}

#ifdef GSHL_TESTS
// #    include "test/mod.h"

GSHL_TEST(write_isize)
{

#    define TEST_WRITE_isize(EXPRESSION, EXPECTED)                             \
        do {                                                                   \
            GSHL_FormatString string = {};                                     \
            GSHL_DArray_init(&string);                                         \
            const GSHL_FormatSpecifier fs = {                                  \
                .kind = GSHL_FORMAT_SPECIFIER_ISIZE,                           \
                .write = GSHL_write_isize,                                     \
                .value.isize = (EXPRESSION),                                   \
            };                                                                 \
                                                                               \
            const usize count = GSHL_write_isize(&string, &fs);                \
            GSHL_DArray_append(&string, '\0');                                 \
            GSHL_TEST_EQUAL(count, GSHL_STACK_STRING_LEN(EXPECTED));           \
            GSHL_TEST_STR_EQUAL(string.items, EXPECTED);                       \
            GSHL_DArray_destroy(&string);                                      \
        } while (0)

    TEST_WRITE_isize(42, "42");
    TEST_WRITE_isize(0, "0");
    TEST_WRITE_isize(294482, "294482");
    TEST_WRITE_isize(-294482, "-294482");
    TEST_WRITE_isize(~0, "-1");

#    undef TEST_WRITE_isize
}

#endif
#ifdef GSHL_SOURCE_CODE_MAPPING
#    line 0 "src/format/i8.c"
#endif // GSHL_SOURCE_CODE_MAPPING
// #include "format/i8.h"

// #include "format/isize.h"
// #include "macros/mod.h"
// #include "string/mod.h"

#include <assert.h>
#include <stdlib.h>

usize GSHL_write_i8(GSHL_FormatString *string, const GSHL_FormatSpecifier *fs)
{
    GSHL_ASSERT(fs->kind == GSHL_FORMAT_SPECIFIER_I8);
    GSHL_ASSERT(fs->write == GSHL_write_i8);

    return GSHL_write_isize(string, fs);
}

#ifdef GSHL_TESTS
// #    include "test/mod.h"

GSHL_TEST(write_i8)
{

#    define TEST_WRITE_i8(EXPRESSION, EXPECTED)                                \
        do {                                                                   \
            GSHL_FormatString string = {};                                     \
            GSHL_DArray_init(&string);                                         \
            const GSHL_FormatSpecifier fs = {                                  \
                .kind = GSHL_FORMAT_SPECIFIER_I8,                              \
                .write = GSHL_write_i8,                                        \
                .value.i8 = (EXPRESSION),                                      \
            };                                                                 \
                                                                               \
            const usize count = GSHL_write_i8(&string, &fs);                   \
            GSHL_DArray_append(&string, '\0');                                 \
            GSHL_TEST_EQUAL(count, GSHL_STACK_STRING_LEN(EXPECTED));           \
            GSHL_TEST_STR_EQUAL(string.items, EXPECTED);                       \
            GSHL_DArray_destroy(&string);                                      \
        } while (0)

    TEST_WRITE_i8(42, "42");
    TEST_WRITE_i8(0, "0");
    TEST_WRITE_i8(127, "127");
    TEST_WRITE_i8(-127, "-127");
    TEST_WRITE_i8(~0, "-1");

#    undef TEST_WRITE_i8
}

#endif
#ifdef GSHL_SOURCE_CODE_MAPPING
#    line 0 "src/format/i64.c"
#endif // GSHL_SOURCE_CODE_MAPPING
// #include "format/i64.h"

// #include "format/isize.h"
// #include "macros/mod.h"
// #include "string/mod.h"

#include <assert.h>
#include <stdlib.h>

usize GSHL_write_i64(GSHL_FormatString *string, const GSHL_FormatSpecifier *fs)
{
    GSHL_ASSERT(fs->kind == GSHL_FORMAT_SPECIFIER_I64);
    GSHL_ASSERT(fs->write == GSHL_write_i64);

    return GSHL_write_isize(string, fs);
}

#ifdef GSHL_TESTS
// #    include "test/mod.h"

GSHL_TEST(write_i64)
{

#    define TEST_WRITE_i64(EXPRESSION, EXPECTED)                               \
        do {                                                                   \
            GSHL_FormatString string = {};                                     \
            GSHL_DArray_init(&string);                                         \
            const GSHL_FormatSpecifier fs = {                                  \
                .kind = GSHL_FORMAT_SPECIFIER_I64,                             \
                .write = GSHL_write_i64,                                       \
                .value.i64 = (EXPRESSION),                                     \
            };                                                                 \
                                                                               \
            const usize count = GSHL_write_i64(&string, &fs);                  \
            GSHL_DArray_append(&string, '\0');                                 \
            GSHL_TEST_EQUAL(count, GSHL_STACK_STRING_LEN(EXPECTED));           \
            GSHL_TEST_STR_EQUAL(string.items, EXPECTED);                       \
            GSHL_DArray_destroy(&string);                                      \
        } while (0)

    TEST_WRITE_i64(42, "42");
    TEST_WRITE_i64(0, "0");
    TEST_WRITE_i64(294482, "294482");
    TEST_WRITE_i64(-294482, "-294482");
    TEST_WRITE_i64(~0, "-1");

#    undef TEST_WRITE_i64
}

#endif
#ifdef GSHL_SOURCE_CODE_MAPPING
#    line 0 "src/format/i32.c"
#endif // GSHL_SOURCE_CODE_MAPPING
// #include "format/i32.h"

// #include "format/isize.h"
// #include "macros/mod.h"
// #include "string/mod.h"

#include <assert.h>
#include <stdlib.h>

usize GSHL_write_i32(GSHL_FormatString *string, const GSHL_FormatSpecifier *fs)
{
    GSHL_ASSERT(fs->kind == GSHL_FORMAT_SPECIFIER_I32);
    GSHL_ASSERT(fs->write == GSHL_write_i32);

    return GSHL_write_isize(string, fs);
}

#ifdef GSHL_TESTS
// #    include "test/mod.h"

GSHL_TEST(write_i32)
{

#    define TEST_WRITE_i32(EXPRESSION, EXPECTED)                               \
        do {                                                                   \
            GSHL_FormatString string = {};                                     \
            GSHL_DArray_init(&string);                                         \
            const GSHL_FormatSpecifier fs = {                                  \
                .kind = GSHL_FORMAT_SPECIFIER_I32,                             \
                .write = GSHL_write_i32,                                       \
                .value.i32 = (EXPRESSION),                                     \
            };                                                                 \
                                                                               \
            const usize count = GSHL_write_i32(&string, &fs);                  \
            GSHL_DArray_append(&string, '\0');                                 \
            GSHL_TEST_EQUAL(count, GSHL_STACK_STRING_LEN(EXPECTED));           \
            GSHL_TEST_STR_EQUAL(string.items, EXPECTED);                       \
            GSHL_DArray_destroy(&string);                                      \
        } while (0)

    TEST_WRITE_i32(42, "42");
    TEST_WRITE_i32(0, "0");
    TEST_WRITE_i32(294482, "294482");
    TEST_WRITE_i32(-294482, "-294482");
    TEST_WRITE_i32(~0, "-1");

#    undef TEST_WRITE_i32
}

#endif
#ifdef GSHL_SOURCE_CODE_MAPPING
#    line 0 "src/format/i16.c"
#endif // GSHL_SOURCE_CODE_MAPPING
// #include "format/i16.h"

// #include "format/isize.h"
// #include "macros/mod.h"
// #include "string/mod.h"

#include <assert.h>
#include <stdlib.h>

usize GSHL_write_i16(GSHL_FormatString *string, const GSHL_FormatSpecifier *fs)
{
    GSHL_ASSERT(fs->kind == GSHL_FORMAT_SPECIFIER_I16);
    GSHL_ASSERT(fs->write == GSHL_write_i16);

    return GSHL_write_isize(string, fs);
}

#ifdef GSHL_TESTS
// #    include "test/mod.h"

GSHL_TEST(write_i16)
{

#    define TEST_WRITE_i16(EXPRESSION, EXPECTED)                               \
        do {                                                                   \
            GSHL_FormatString string = {};                                     \
            GSHL_DArray_init(&string);                                         \
            const GSHL_FormatSpecifier fs = {                                  \
                .kind = GSHL_FORMAT_SPECIFIER_I16,                             \
                .write = GSHL_write_i16,                                       \
                .value.i16 = (EXPRESSION),                                     \
            };                                                                 \
                                                                               \
            const usize count = GSHL_write_i16(&string, &fs);                  \
            GSHL_DArray_append(&string, '\0');                                 \
            GSHL_TEST_EQUAL(count, GSHL_STACK_STRING_LEN(EXPECTED));           \
            GSHL_TEST_STR_EQUAL(string.items, EXPECTED);                       \
            GSHL_DArray_destroy(&string);                                      \
        } while (0)

    TEST_WRITE_i16(42, "42");
    TEST_WRITE_i16(0, "0");
    TEST_WRITE_i16(2942, "2942");
    TEST_WRITE_i16(-2942, "-2942");
    TEST_WRITE_i16(~0, "-1");

#    undef TEST_WRITE_i16
}

#endif
#ifdef GSHL_SOURCE_CODE_MAPPING
#    line 0 "src/format/hex.c"
#endif // GSHL_SOURCE_CODE_MAPPING
// #include "format/hex.h"
// #include "format/pointer.h"

// #include "array/mod.h"
// #include "format/usize.h"
// #include "macros/mod.h"
// #include "string/mod.h"

#include <assert.h>
#include <stdlib.h>

usize GSHL_write_hex32(GSHL_FormatString *string,
                       const GSHL_FormatSpecifier *fs)
{
    GSHL_ASSERT(fs->kind == GSHL_FORMAT_SPECIFIER_HEX32);
    GSHL_ASSERT(fs->write == GSHL_write_hex32);

    GSHL_FormatSpecifier fs_copy = *fs;
    fs_copy.value.hex64 = fs->value.hex32;

    return GSHL_write_hex64(string, &fs_copy);
}

usize GSHL_write_hex64(GSHL_FormatString *string,
                       const GSHL_FormatSpecifier *fs)
{
    GSHL_ASSERT(fs->kind &
                (GSHL_FORMAT_SPECIFIER_HEX32 | GSHL_FORMAT_SPECIFIER_HEX64 |
                 GSHL_FORMAT_SPECIFIER_POINTER));
    GSHL_ASSERT(fs->write == GSHL_write_hex32 ||
                fs->write == GSHL_write_hex64 ||
                fs->write == GSHL_write_pointer);

    static const char conversion[][17] = {
        [false] = "0123456789abcdef",
        [true] = "0123456789ABCDEF",
    };

    const char *convert =
        conversion[fs->opts.hex64.uppercase % GSHL_ARRAY_LEN(conversion)];

    usize count = 0;
    hex64 rem = fs->value.hex64;
    do {
        if (string != NULL) {
            GSHL_DArray_append(string, convert[rem % 16]);
        }

        rem >>= 4;
        count += 1;
    } while (rem != 0);

    if (string != NULL) {
        GSHL_string_reverse(&string->items[string->count - count],
                            &string->items[string->count]);
    }

    return count;
}

#ifdef GSHL_TESTS
// #    include "test/mod.h"

GSHL_TEST(write_hex32)
{

#    define TEST_WRITE_hex32(EXPRESSION, EXPECTED, ...)                        \
        do {                                                                   \
            GSHL_FormatString string = {};                                     \
            GSHL_DArray_init(&string);                                         \
            const GSHL_FormatSpecifier fs = {                                  \
                .kind = GSHL_FORMAT_SPECIFIER_HEX32,                           \
                .write = GSHL_write_hex32,                                     \
                .value.hex32 = (EXPRESSION),                                   \
                .opts.hex32 = {__VA_ARGS__},                                   \
            };                                                                 \
                                                                               \
            const hex32 count = GSHL_write_hex32(&string, &fs);                \
            GSHL_DArray_append(&string, '\0');                                 \
            GSHL_TEST_EQUAL(count, GSHL_STACK_STRING_LEN(EXPECTED));           \
            GSHL_TEST_STR_EQUAL(string.items, EXPECTED);                       \
            GSHL_DArray_destroy(&string);                                      \
        } while (0)

#    define TEST_WRITE_hex64(EXPRESSION, EXPECTED, ...)                        \
        do {                                                                   \
            GSHL_FormatString string = {};                                     \
            GSHL_DArray_init(&string);                                         \
            const GSHL_FormatSpecifier fs = {                                  \
                .kind = GSHL_FORMAT_SPECIFIER_HEX64,                           \
                .write = GSHL_write_hex64,                                     \
                .value.hex64 = (EXPRESSION),                                   \
                .opts.hex64 = {__VA_ARGS__},                                   \
            };                                                                 \
                                                                               \
            const hex64 count = GSHL_write_hex64(&string, &fs);                \
            GSHL_DArray_append(&string, '\0');                                 \
            GSHL_TEST_EQUAL(count, GSHL_STACK_STRING_LEN(EXPECTED));           \
            GSHL_TEST_STR_EQUAL(string.items, EXPECTED);                       \
            GSHL_DArray_destroy(&string);                                      \
        } while (0)

    TEST_WRITE_hex32(0xFF, "ff");
    TEST_WRITE_hex32(0xFF, "FF", .uppercase = true);
    TEST_WRITE_hex64(0xFF, "ff");
    TEST_WRITE_hex64(0xFF, "FF", .uppercase = true);

#    undef TEST_WRITE_hex32
}

#endif
#ifdef GSHL_SOURCE_CODE_MAPPING
#    line 0 "src/format/cstring.c"
#endif // GSHL_SOURCE_CODE_MAPPING
// #include "format/cstring.h"

// #include "macros/mod.h"
// #include "string/mod.h"

#include <assert.h>
#include <stdlib.h>

usize GSHL_write_cstring(GSHL_FormatString *string,
                         const GSHL_FormatSpecifier *fs)
{
    GSHL_ASSERT(fs->kind == GSHL_FORMAT_SPECIFIER_CSTRING);
    GSHL_ASSERT(fs->write == GSHL_write_cstring);

    const usize start_count = string->count;

    GSHL_STRING_FOREACH(fs->value.cstring, c) { GSHL_DArray_append(string, c); }

    return string->count - start_count;
}

#ifdef GSHL_TESTS
// #    include "test/mod.h"

GSHL_TEST(write_cstring)
{

#    define TEST_WRITE_cstring(EXPRESSION, EXPECTED)                           \
        do {                                                                   \
            GSHL_FormatString string = {};                                     \
            GSHL_DArray_init(&string);                                         \
            const GSHL_FormatSpecifier fs = {                                  \
                .kind = GSHL_FORMAT_SPECIFIER_CSTRING,                         \
                .write = GSHL_write_cstring,                                   \
                .value.cstring = (EXPRESSION),                                 \
            };                                                                 \
                                                                               \
            const usize count = GSHL_write_cstring(&string, &fs);              \
            GSHL_DArray_append(&string, '\0');                                 \
            GSHL_TEST_EQUAL(count, GSHL_STACK_STRING_LEN(EXPECTED));           \
            GSHL_TEST_STR_EQUAL(string.items, EXPECTED);                       \
            GSHL_DArray_destroy(&string);                                      \
        } while (0)

    TEST_WRITE_cstring("", "");
    TEST_WRITE_cstring("Hello world!", "Hello world!");
    TEST_WRITE_cstring("foo bar baz", "foo bar baz");

#    undef TEST_WRITE_cstring
}

#endif
#ifdef GSHL_SOURCE_CODE_MAPPING
#    line 0 "src/format/char.c"
#endif // GSHL_SOURCE_CODE_MAPPING
// #include "format/char.h"

// #include "macros/mod.h"
// #include "string/mod.h"

#include <assert.h>
#include <stdlib.h>

usize GSHL_write_char(GSHL_FormatString *string, const GSHL_FormatSpecifier *fs)
{
    GSHL_ASSERT(fs->kind == GSHL_FORMAT_SPECIFIER_CHAR);
    GSHL_ASSERT(fs->write == GSHL_write_char);

    const char c = fs->value.character;
    if (string != NULL) {
        GSHL_DArray_append(string, c);
    }

    return 1;
}

#ifdef GSHL_TESTS
// #    include "test/mod.h"

GSHL_TEST(write_char)
{

#    define TEST_WRITE_char(EXPRESSION, EXPECTED)                              \
        do {                                                                   \
            GSHL_FormatString string = {};                                     \
            GSHL_DArray_init(&string);                                         \
            const GSHL_FormatSpecifier fs = {                                  \
                .kind = GSHL_FORMAT_SPECIFIER_CHAR,                            \
                .write = GSHL_write_char,                                      \
                .value.character = (EXPRESSION),                               \
            };                                                                 \
                                                                               \
            const usize count = GSHL_write_char(&string, &fs);                 \
            const char expected[] = EXPECTED;                                  \
            GSHL_DArray_append(&string, '\0');                                 \
            GSHL_TEST_EQUAL(count, 1);                                         \
            GSHL_TEST_STR_EQUAL(string.items, expected);                       \
            GSHL_DArray_destroy(&string);                                      \
        } while (0)

    TEST_WRITE_char(0, "");
    TEST_WRITE_char('0', "0");
    TEST_WRITE_char('1', "1");
    TEST_WRITE_char('2', "2");
    TEST_WRITE_char('3', "3");
    TEST_WRITE_char('4', "4");
    TEST_WRITE_char('%', "%");
    TEST_WRITE_char('\0', "");

#    undef TEST_WRITE_char
}

#endif
#ifdef GSHL_SOURCE_CODE_MAPPING
#    line 0 "src/format/bool.c"
#endif // GSHL_SOURCE_CODE_MAPPING
// #include "format/bool.h"

// #include "macros/mod.h"
// #include "string/mod.h"

#include <assert.h>
#include <stdlib.h>

usize GSHL_write_bool(GSHL_FormatString *string, const GSHL_FormatSpecifier *fs)
{
    GSHL_ASSERT(fs->kind == GSHL_FORMAT_SPECIFIER_BOOL);
    GSHL_ASSERT(fs->write == GSHL_write_bool);

    const bool boolean = !!fs->value.boolean;
    GSHL_ASSERT(boolean == 1 || boolean == 0);

    static const char conversions[][6] = {
        [false] = "false",
        [true] = "true",
    };

    usize count = 0;
    GSHL_STRING_FOREACH(conversions[boolean], c)
    {
        if (string != NULL) {
            GSHL_DArray_append(string, c);
        }

        count += 1;
    }

    return count;
}

#ifdef GSHL_TESTS
// #    include "test/mod.h"

GSHL_TEST(write_bool)
{

#    define TEST_WRITE_bool(EXPRESSION, EXPECTED, ...)                         \
        do {                                                                   \
            GSHL_FormatString string = {};                                     \
            GSHL_DArray_init(&string);                                         \
            const GSHL_FormatSpecifier fs = {                                  \
                .kind = GSHL_FORMAT_SPECIFIER_BOOL,                            \
                .write = GSHL_write_bool,                                      \
                .value.boolean = (EXPRESSION),                                 \
                .opts.boolean = {__VA_ARGS__},                                 \
            };                                                                 \
                                                                               \
            const usize count = GSHL_write_bool(&string, &fs);                 \
            GSHL_DArray_append(&string, '\0');                                 \
            GSHL_TEST_EQUAL(count, GSHL_STACK_STRING_LEN(EXPECTED));           \
            GSHL_TEST_STR_EQUAL(string.items, EXPECTED);                       \
            GSHL_DArray_destroy(&string);                                      \
        } while (0)

    TEST_WRITE_bool(false, "false");
    TEST_WRITE_bool(true, "true");
    TEST_WRITE_bool(0, "false");
    TEST_WRITE_bool(1, "true");
    TEST_WRITE_bool(~0, "true");
    TEST_WRITE_bool(~1, "true");
    TEST_WRITE_bool(~~0, "false");
    TEST_WRITE_bool(!0, "true");
    TEST_WRITE_bool(!1, "false");

#    undef TEST_WRITE_bool
}

#endif
#ifdef GSHL_SOURCE_CODE_MAPPING
#    line 0 "src/dynamic_array/mod.c"
#endif // GSHL_SOURCE_CODE_MAPPING
// #include "dynamic_array/mod.h"

// #include "array/mod.h"

#include <assert.h>
#include <stdlib.h>

#ifdef GSHL_TESTS
// #    include "test/mod.h"

GSHL_DArrayTypeDecl(DArray, int);

GSHL_TEST(da)
{
    DArray da = {};
    GSHL_TEST_EQUAL(da.items, NULL);
    GSHL_TEST_EQUAL(da.capacity, 0);
    GSHL_TEST_EQUAL(da.size, 0);

    GSHL_DArray_init(&da);
    GSHL_TEST_EQUAL(da.items, NULL);
    GSHL_TEST_EQUAL(da.capacity, 0);
    GSHL_TEST_EQUAL(da.size, 0);

    GSHL_DArray_append(&da, 10);
    GSHL_TEST_NEQUAL(da.items, NULL);
    GSHL_TEST_EQUAL(da.capacity, GSHL_DARRAY_INIT_CAPACITY);
    GSHL_TEST_EQUAL(da.size, 1);

    GSHL_DArray_append(&da, 67);
    GSHL_DArray_append(&da, 420);
    GSHL_DArray_append(&da, 69);
    GSHL_TEST_NEQUAL(da.items, NULL);
    GSHL_TEST_EQUAL(da.items[0], 10);
    GSHL_TEST_EQUAL(da.items[1], 67);
    GSHL_TEST_EQUAL(da.items[2], 420);
    GSHL_TEST_EQUAL(da.items[3], 69);
    GSHL_TEST_EQUAL(da.capacity, GSHL_DARRAY_INIT_CAPACITY);
    GSHL_TEST_EQUAL(da.size, 4);

    GSHL_DArray_insert(&da, 2, 12345);
    GSHL_TEST_NEQUAL(da.items, NULL);
    GSHL_TEST_EQUAL(da.items[0], 10);
    GSHL_TEST_EQUAL(da.items[1], 67);
    GSHL_TEST_EQUAL(da.items[2], 12345);
    GSHL_TEST_EQUAL(da.items[3], 420);
    GSHL_TEST_EQUAL(da.items[4], 69);
    GSHL_TEST_EQUAL(da.capacity, GSHL_DARRAY_INIT_CAPACITY);
    GSHL_TEST_EQUAL(da.size, 5);

    GSHL_DArray_remove(&da, 1);
    GSHL_TEST_NEQUAL(da.items, NULL);
    GSHL_TEST_EQUAL(da.items[0], 10);
    GSHL_TEST_EQUAL(da.items[1], 12345);
    GSHL_TEST_EQUAL(da.items[2], 420);
    GSHL_TEST_EQUAL(da.items[3], 69);
    GSHL_TEST_EQUAL(da.capacity, GSHL_DARRAY_INIT_CAPACITY);
    GSHL_TEST_EQUAL(da.size, 4);

    GSHL_DArray_init(&da);
    GSHL_TEST_NEQUAL(da.items, NULL);
    GSHL_TEST_NEQUAL(da.capacity, 0);
    GSHL_TEST_EQUAL(da.size, 0);

    for (usize n = 1; n <= 3 * GSHL_DARRAY_INIT_CAPACITY; n++) {
        GSHL_DArray_append(&da, n);
    }

    GSHL_TEST_NEQUAL(da.items, NULL);
    GSHL_TEST_EQUAL(da.capacity, 4 * GSHL_DARRAY_INIT_CAPACITY);
    GSHL_TEST_EQUAL(da.count, 3 * GSHL_DARRAY_INIT_CAPACITY);

    GSHL_DArray_destroy(&da);
    GSHL_TEST_EQUAL(da.items, NULL);
    GSHL_TEST_EQUAL(da.capacity, 0);
    GSHL_TEST_EQUAL(da.count, 0);

    const int elements[] = {1, 2, 3};
    GSHL_DArray_extend(&da, elements);
    GSHL_TEST_NEQUAL(da.items, NULL);
    GSHL_TEST_EQUAL(da.capacity, GSHL_DARRAY_INIT_CAPACITY);
    GSHL_TEST_EQUAL(da.count, GSHL_ARRAY_LEN(elements));
    GSHL_TEST_EQUAL(da.items[0], 1);
    GSHL_TEST_EQUAL(da.items[1], 2);
    GSHL_TEST_EQUAL(da.items[2], 3);

    const int elements2[] = {-1, -2, -3};
    GSHL_DArray_extend(&da, elements2);
    GSHL_TEST_NEQUAL(da.items, NULL);
    GSHL_TEST_EQUAL(da.capacity, GSHL_DARRAY_INIT_CAPACITY);
    GSHL_TEST_EQUAL(da.count,
                    GSHL_ARRAY_LEN(elements) + GSHL_ARRAY_LEN(elements2));
    GSHL_TEST_EQUAL(da.items[0], 1);
    GSHL_TEST_EQUAL(da.items[1], 2);
    GSHL_TEST_EQUAL(da.items[2], 3);
    GSHL_TEST_EQUAL(da.items[3], -1);
    GSHL_TEST_EQUAL(da.items[4], -2);
    GSHL_TEST_EQUAL(da.items[5], -3);

    GSHL_DArray_destroy(&da);
}

#endif
#ifdef GSHL_SOURCE_CODE_MAPPING
#    line 0 "src/array/mod.c"
#endif // GSHL_SOURCE_CODE_MAPPING
// #include "array/mod.h"
// #include "types/mod.h"
#include <stdlib.h>

#ifdef GSHL_TESTS

// #    include "test/mod.h"

GSHL_TEST(stackarray)
{
    const u8 a[] = {1, 2, 3, 4, 5};

    GSHL_TEST_EQUAL(GSHL_ARRAY_LEN(a), 5);

    u8 sum = 0;
    GSHL_ARRAY_FOREACH(a, u8 element) { sum += element; }
    GSHL_TEST_EQUAL(sum, 15);
}

GSHL_TEST(array)
{
    const struct {
        i32 *items;
        usize count;
    } array = {
        .count = 5,
        .items = calloc(5, sizeof(i32)),
    };

    GSHL_ARRAYN_FOREACH_MUT(array.items, array.count, i32 * element)
    {
        *element = index + 1;
    }

    u8 sum = 0;
    GSHL_ARRAYN_FOREACH(array.items, array.count, i32 element)
    {
        sum += element;
    }
    GSHL_TEST_EQUAL(sum, 15);

    free(array.items);
}

#endif
#undef GSHL_IMPLEMENTATION
#endif // GSHL_IMPLEMENTATION
