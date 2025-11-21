#ifndef INCLUDE_FORMAT_ISIZE_H_
#define INCLUDE_FORMAT_ISIZE_H_

#include "format/mod.h"
#include "types/mod.h"

/// {{{ Functions

usize write_isize(char *buf, const isize value,
                  const struct GSHL_TemplateOpts_isize opts,
                  const usize precomputed_count);

/// }}}

#endif // INCLUDE_FORMAT_ISIZE_H_
