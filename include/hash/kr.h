#ifndef INCLUDE_HASH_KR_H_
#define INCLUDE_HASH_KR_H_

#include "types/mod.h"

/// {{{ Functions

usize GSHL_hash_kr(const char *const string);

/// }}}

#ifdef GSHL_STRIP_PREFIX
#    define hash_kr GSHL_hash_kr
#endif

#endif // INCLUDE_HASH_KR_H_
