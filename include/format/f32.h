#ifndef INCLUDE_FORMAT_F32_H_
#define INCLUDE_FORMAT_F32_H_

#include "format/mod.h"
#include "types/mod.h"

/// {{{ Functions

usize write_f32(char *buf, const f32 value,
                const struct GSHL_TemplateOpts_f32 opts,
                const usize precomputed_count);

/// }}}

#endif // INCLUDE_FORMAT_F32_H_
