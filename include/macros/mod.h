// gshl-priority: 99
#ifndef INCLUDE_MACROS_MOD_H_
#define INCLUDE_MACROS_MOD_H_

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

#ifdef GSHL_STRIP_PREFIX
#    define ASSERT GSHL_ASSERT
#    define loop GSHL_LOOP
#    define LOOP GSHL_LOOP
#    define TODO GSHL_TODO
#    define UNREACHABLE GSHL_UNREACHABLE
#    define UNUSED GSHL_UNUSED
#endif

#endif // INCLUDE_MACROS_MOD_H_
