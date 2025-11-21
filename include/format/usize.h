#ifndef INCLUDE_FORMAT_USIZE_H_
#define INCLUDE_FORMAT_USIZE_H_

#include "format/mod.h"
#include "types/mod.h"

/// {{{ Functions

usize write_usize(char *buf, const usize value,
                  const struct GSHL_TemplateOpts_usize opts,
                  const usize precomputed_count);

/// }}}

#endif // INCLUDE_FORMAT_USIZE_H_
