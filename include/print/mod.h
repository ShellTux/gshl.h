// gshl-priority: 80
#ifndef INCLUDE_PRINT_MOD_H_
#define INCLUDE_PRINT_MOD_H_

#include "format/mod.h"
#include "types/mod.h"

#include <stdarg.h>

/// {{{ Types

typedef struct GSHL_PrintWrapperOpts {
    usize fd;
} GSHL_PrintWrapperOpts;

/// }}}

/// {{{ Functions

struct GSHL_FormatWrapperOpts;
usize GSHL_print_wrapperv(const GSHL_PrintWrapperOpts print_opts,
                          const struct GSHL_FormatWrapperOpts format_opts,
                          const char *restrict format, va_list args);

usize GSHL_print(const char *restrict format, ...);
usize GSHL_dprint(const int fd, const char *restrict format, ...);
usize GSHL_eprint(const char *restrict format, ...);
usize GSHL_println(const char *restrict format, ...);
usize GSHL_dprintln(const int fd, const char *restrict format, ...);
usize GSHL_eprintln(const char *restrict format, ...);

/// }}}

#ifdef GSHL_STRIP_PREFIX
#    define print GSHL_print
#    define dprint GSHL_dprint
#    define println GSHL_println
#    define dprintln GSHL_dprintln
#endif

#endif // INCLUDE_PRINT_MOD_H_
