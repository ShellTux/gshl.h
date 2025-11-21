#ifndef INCLUDE_PRINT_MOD_H_
#define INCLUDE_PRINT_MOD_H_

#include "format/mod.h"
#include "types/mod.h"

/// {{{ Functions

usize GSHL_print(const char *restrict format, ...);

/// }}}

#ifdef GSHL_STRIP_PREFIX
#    define print GSHL_print
#endif

#endif // INCLUDE_PRINT_MOD_H_
