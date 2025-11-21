#ifndef INCLUDE_FORMAT_I32_H_
#define INCLUDE_FORMAT_I32_H_

#include "format/mod.h"
#include "types/mod.h"

/// {{{ Functions

usize write_i32(char *buf, const i32 i32,
                const struct GSHL_TemplateOpts_i32 opts,
                const usize precomputed_count);

/// }}}

#endif // INCLUDE_FORMAT_I32_H_
