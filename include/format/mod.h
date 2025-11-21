#ifndef INCLUDE_FORMAT_MOD_H_
#define INCLUDE_FORMAT_MOD_H_

#include "format/wrapper.h"

/// {{{ Functions

char *GSHL_format(const char *restrict format, ...);

/// }}}

#ifdef GSHL_STRIP_PREFIX
#define format GSHL_format
#endif

#endif // INCLUDE_FORMAT_MOD_H_
