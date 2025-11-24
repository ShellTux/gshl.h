#ifndef INCLUDE_HASH_FNV1A_H_
#define INCLUDE_HASH_FNV1A_H_

#include "types/mod.h"

/// {{{ Functions

u32 GSHL_hash_fnv1a32(const char *const input);

/// }}}

#ifdef GSHL_STRIP_PREFIX
#    define hash_fnv1a32 GSHL_hash_fnv1a32
#endif

#endif // INCLUDE_HASH_FNV1A_H_
