#ifndef INCLUDE_FORMAT_POINTER_H_
#define INCLUDE_FORMAT_POINTER_H_

#include "types/mod.h"

/// {{{ Functions

usize write_pointer(char *buf, const void *pointer,
                    const usize precomputed_count);

/// }}}

#endif // INCLUDE_FORMAT_POINTER_H_
