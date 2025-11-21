#ifndef INCLUDE_FORMAT_I8_H_
#define INCLUDE_FORMAT_I8_H_

#include "format/mod.h"
#include "types/mod.h"

/// {{{ Functions

usize write_i8(char *buf, const i8 i8,
        const struct GSHL_TemplateOpts_i8 opts,
        const usize precomputed_count);

/// }}}

#endif // INCLUDE_FORMAT_I8_H_
