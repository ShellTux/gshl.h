#ifndef INCLUDE_FORMAT_F64_H_
#define INCLUDE_FORMAT_F64_H_

#include "format/mod.h"
#include "types/mod.h"

/// {{{ Functions

usize write_f64(char *buf, const f64 value,
                const struct GSHL_TemplateOpts_f64 opts,
                const usize precomputed_count);

/// }}}

#endif // INCLUDE_FORMAT_F64_H_
