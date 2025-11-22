#ifndef INCLUDE_PRINT_MOD_H_
#define INCLUDE_PRINT_MOD_H_

#include "format/mod.h"
#include "types/mod.h"

#include <stdarg.h>

/// {{{ Functions

usize GSHL_print(const char *restrict format, ...);
usize GSHL_printv(const char *restrict format, va_list args);
usize GSHL_println(const char *restrict format, ...);

/// }}}

#ifdef GSHL_STRIP_PREFIX
#    define print GSHL_print
#    define println GSHL_println
#endif

#endif // INCLUDE_PRINT_MOD_H_
