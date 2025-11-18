#ifndef INCLUDE_INCLUDE_ARRAY_H_
#define INCLUDE_INCLUDE_ARRAY_H_

#include "types/mod.h"

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

#endif // INCLUDE_INCLUDE_ARRAY_H_
