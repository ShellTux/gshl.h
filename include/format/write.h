// gshl-priority: 70
#ifndef INCLUDE_FORMAT_WRITE_H_
#define INCLUDE_FORMAT_WRITE_H_

#include "format/register.h"

#include "types/mod.h"

#include <stdarg.h>

/// {{{ Functions

usize GSHL_format_write(GSHL_FormatString *string,
                        const char *const restrict format, ...);
usize GSHL_format_writev(GSHL_FormatString *string,
                         const char *const restrict format, va_list args);

/// }}}

#endif // INCLUDE_FORMAT_WRITE_H_
