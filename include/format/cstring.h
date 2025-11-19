#ifndef INCLUDE_FORMAT_CSTRING_H_
#define INCLUDE_FORMAT_CSTRING_H_

#include "types/mod.h"

/// {{{ Functions

usize write_cstring(char *buf, const char *cstring,
                    const usize precomputed_count);

/// }}}

#endif // INCLUDE_FORMAT_CSTRING_H_
