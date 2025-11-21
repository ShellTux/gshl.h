#ifndef INCLUDE_FORMAT_U32_H_
#define INCLUDE_FORMAT_U32_H_

#include "format/mod.h"
#include "types/mod.h"

/// {{{ Functions

usize write_u32(char *buf, const u32 u32,
                const struct GSHL_TemplateOpts_u32 opts,
                const usize precomputed_count);

/// }}}

#endif // INCLUDE_FORMAT_U32_H_
