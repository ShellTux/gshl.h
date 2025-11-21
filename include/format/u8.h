#ifndef INCLUDE_FORMAT_U8_H_
#define INCLUDE_FORMAT_U8_H_

#include "format/mod.h"
#include "types/mod.h"

/// {{{ Functions

usize write_u8(char *buf, const u8 value,
               const struct GSHL_TemplateOpts_u8 opts,
               const usize precomputed_count);

/// }}}

#endif // INCLUDE_FORMAT_U8_H_
