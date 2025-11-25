#ifndef INCLUDE_PRINT_MOD_H_
#define INCLUDE_PRINT_MOD_H_

#include "types/mod.h"

#include <stdarg.h>

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

#endif // INCLUDE_PRINT_MOD_H_
