// gshl-priority: 85
#ifndef INCLUDE_DYNAMIC_ARRAY_MOD_H_
#define INCLUDE_DYNAMIC_ARRAY_MOD_H_

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
        if ((DARRAY)->items == NULL) {                                         \
            (DARRAY)->capacity = GSHL_DARRAY_INIT_CAPACITY;                    \
            (DARRAY)->items =                                                  \
                calloc((DARRAY)->capacity, sizeof(*(DARRAY)->items));          \
            GSHL_ASSERT((DARRAY)->items != NULL);                              \
        }                                                                      \
                                                                               \
        if ((DARRAY)->count + 1 > (DARRAY)->capacity) {                        \
            (DARRAY)->capacity *= 2;                                           \
            (DARRAY)->items =                                                  \
                realloc((DARRAY)->items,                                       \
                        (DARRAY)->capacity * sizeof(*(DARRAY)->items));        \
            assert((DARRAY)->items != NULL);                                   \
        }                                                                      \
                                                                               \
        (DARRAY)->items[(DARRAY)->count++] = ITEM;                             \
    } while (0)

#define GSHL_DArray_insert(DARRAY, INDEX, ITEM)                                \
    do {                                                                       \
        assert(0 <= INDEX && INDEX <= (DARRAY)->count);                        \
        if ((DARRAY)->count >= (DARRAY)->capacity) {                           \
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

#endif // INCLUDE_DYNAMIC_ARRAY_MOD_H_
