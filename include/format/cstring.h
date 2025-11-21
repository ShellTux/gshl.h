#ifndef INCLUDE_FORMAT_CSTRING_H_
#define INCLUDE_FORMAT_CSTRING_H_

#include "format/mod.h"
#include "types/mod.h"

/// {{{ Functions

usize write_cstring(char *buf, const char *cstring,
                    const struct GSHL_TemplateOpts_cstring opts,
                    const usize precomputed_count);

/// }}}

#endif // INCLUDE_FORMAT_CSTRING_H_
