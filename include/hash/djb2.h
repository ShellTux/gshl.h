#ifndef INCLUDE_HASH_DJB2_H_
#define INCLUDE_HASH_DJB2_H_

#include "types/mod.h"

/// {{{ Functions

usize GSHL_hash_djb2(const char *string);

/// }}}

#ifdef GSHL_STRIP_PREFIX
#    define hash_djb2 GSHL_hash_djb2
#endif

#endif // INCLUDE_HASH_DJB2_H_
