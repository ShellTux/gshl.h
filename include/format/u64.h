#ifndef INCLUDE_FORMAT_U64_H_
#define INCLUDE_FORMAT_U64_H_

#include "format/mod.h"
#include "types/mod.h"

/// {{{ Functions

usize write_u64(char *buf, const u64 value,
                const struct GSHL_TemplateOpts_u64 opts,
                const usize precomputed_count);

/// }}}

#endif // INCLUDE_FORMAT_U64_H_
