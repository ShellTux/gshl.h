#ifndef INCLUDE_FORMAT_HEX_H_
#define INCLUDE_FORMAT_HEX_H_

#include "format/mod.h"
#include "types/mod.h"

/// {{{ Functions

usize write_hex(char *buf, const usize hex,
                const struct GSHL_TemplateOpts_hex opts,
                const usize precomputed_count);

/// }}}

#endif // INCLUDE_FORMAT_HEX_H_
