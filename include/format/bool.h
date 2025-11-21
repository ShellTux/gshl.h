#ifndef INCLUDE_FORMAT_BOOL_H_
#define INCLUDE_FORMAT_BOOL_H_

#include "format/mod.h"
#include "types/mod.h"

/// {{{ Functions

usize write_boolean(char *buf, const bool boolean,
                    const struct GSHL_TemplateOpts_boolean opts,
                    const usize precomputed_count);

/// }}}

#endif // INCLUDE_FORMAT_BOOL_H_
