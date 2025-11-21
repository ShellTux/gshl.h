#ifndef INCLUDE_FORMAT_I64_H_
#define INCLUDE_FORMAT_I64_H_

#include "format/mod.h"
#include "types/mod.h"

/// {{{ Functions

usize write_i64(char *buf, const i64 i64,
                const struct GSHL_TemplateOpts_i64 opts,
                const usize precomputed_count);

/// }}}

#endif // INCLUDE_FORMAT_I64_H_
