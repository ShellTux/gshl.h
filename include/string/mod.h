#ifndef INCLUDE_STRING_MOD_H_
#define INCLUDE_STRING_MOD_H_

#include "types/mod.h"

/// {{{ Macros

#define GSHL_STACK_STRING_LEN(ARRAY) (sizeof(ARRAY) - 1)

/// }}}

/// {{{ Types

typedef struct GSHL_StringView {
    char *start;
    usize len;
} GSHL_StringView;

/// }}}

/// {{{ Functions

char *GSHL_string_dup(const char *const source);

/// }}}

#ifdef GSHL_TESTS
#    define STACK_STRING_LEN GSHL_STACK_STRING_LEN
#    define StringView GSHL_StringView
#    define string_dup GSHL_string_dup
#endif

#endif // INCLUDE_STRING_MOD_H_
