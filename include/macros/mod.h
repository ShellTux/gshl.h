#ifndef INCLUDE_INCLUDE_MACROS_H_
#define INCLUDE_INCLUDE_MACROS_H_

#include <stdio.h> // IWYU pragma: keep

#ifndef GSHLDEF
#    define GSHLDEF
#endif

#define GSHL_UNUSED(X) (void)(X)
#define GSHL_EXIT(FORMAT, MESSAGE)                                             \
    do {                                                                       \
        fprintf(stderr, "%s:%d: " FORMAT "\n", __FILE__, __LINE__, MESSAGE);   \
        exit(1);                                                               \
    } while (0)
#define GSHL_TODO(MESSAGE) GSHL_EXIT("TODO: %s", MESSAGE)
#define GSHL_UNREACHABLE(MESSAGE) GSHL_EXIT("UNREACHABLE: %s", MESSAGE)
#define GSHL_LOOP for (;;)

#ifdef GSHL_STRIP_PREFIX
#    define loop GSHL_LOOP
#    define LOOP GSHL_LOOP
#    define TODO GSHL_TODO
#    define UNREACHABLE GSHL_UNREACHABLE
#    define UNUSED GSHL_UNUSED
#endif

#endif // INCLUDE_INCLUDE_MACROS_H_
