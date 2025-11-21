#ifndef INCLUDE_FORMAT_BINARY_H_
#define INCLUDE_FORMAT_BINARY_H_

#include "format/mod.h"
#include "types/mod.h"

/// {{{ Functions

usize write_binary(char *buf, const u64 binary,
                   const struct GSHL_TemplateOpts_binary opts,
                   const usize precomputed_count);

/// }}}

#endif // INCLUDE_FORMAT_BINARY_H_
