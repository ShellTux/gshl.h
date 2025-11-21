#ifndef INCLUDE_FORMAT_U16_H_
#define INCLUDE_FORMAT_U16_H_

#include "format/mod.h"
#include "types/mod.h"

/// {{{ Functions

usize write_u16(char *buf, const u16 u16,
                const struct GSHL_TemplateOpts_u16 opts,
                const usize precomputed_count);

/// }}}

#endif // INCLUDE_FORMAT_U16_H_
