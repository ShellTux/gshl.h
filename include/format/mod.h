// gshl-priority: 80
#ifndef INCLUDE_FORMAT_MOD_H_
#define INCLUDE_FORMAT_MOD_H_

#include "string/mod.h"

#include <stdarg.h>
#include <stddef.h>

/// {{{ Functions

usize GSHL_hash_format_specifier(const char *const start, const char *const end,
                                 const char **startP);

GSHL_StringView GSHL_format_wrapper(const char *const restrict format, ...);
GSHL_StringView GSHL_format_wrapperv(const char *const restrict format,
                                     va_list args);

GSHL_NODISCARD char *GSHL_format(const char *const restrict format, ...);
GSHL_NODISCARD char *GSHL_formatv(const char *const restrict format,
                                  va_list args);

/// }}}

#ifdef GSHL_STRIP_PREFIX
#    define format GSHL_format
#    define formatv GSHL_formatv
#    define format_write GSHL_format_write
#    define format_writev GSHL_format_writev
#endif

#endif // INCLUDE_FORMAT_MOD_H_
