#ifndef INCLUDE_FORMAT_I16_H_
#define INCLUDE_FORMAT_I16_H_

#include "format/mod.h"
#include "types/mod.h"

/// {{{ Functions

usize write_i16(char *buf, const i16 i16,
                const struct GSHL_TemplateOpts_i16 opts,
                const usize precomputed_count);

/// }}}

#endif // INCLUDE_FORMAT_I16_H_
