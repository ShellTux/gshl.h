#ifndef INCLUDE_FORMAT_MOD_H_
#define INCLUDE_FORMAT_MOD_H_

#include "format/wrapper.h"

#include <stdarg.h>

/// {{{ Functions

char *GSHL_format(const char *restrict format, ...);
char *GSHL_formatv(const char *restrict format, va_list args);
char *GSHL_formatln(const char *restrict format, ...);

/// }}}

#ifdef GSHL_STRIP_PREFIX
#    define format GSHL_format
#endif

#endif // INCLUDE_FORMAT_MOD_H_
