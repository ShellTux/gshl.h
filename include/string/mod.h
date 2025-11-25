// gshl-priority: 85
#ifndef INCLUDE_STRING_MOD_H_
#define INCLUDE_STRING_MOD_H_

#include "types/mod.h"

/// {{{ Macros

#define GSHL_STACK_STRING_LEN(ARRAY) (sizeof(ARRAY) - 1)
#define GSHL_STRING_FOREACH(STRING, VARNAME)                                   \
    for (usize keep = 1, index = 0, exit = false; keep && !exit;               \
         keep = !keep, index++)                                                \
        for (const char VARNAME = (STRING)[index],                             \
                        *VARNAME##P = &(STRING)[index];                        \
             keep;                                                             \
             keep = !keep && *VARNAME##P != '\0', exit = VARNAME == '\0')

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

#endif // INCLUDE_STRING_MOD_H_
