#ifndef INCLUDE_FORMAT_CHAR_H_
#define INCLUDE_FORMAT_CHAR_H_

#include "format/mod.h"
#include "types/mod.h"

/// {{{ Functions

usize write_character(char *buf, const char character,
                      const struct GSHL_TemplateOpts_character opts,
                      const usize precomputed_count);

/// }}}

#endif // INCLUDE_FORMAT_CHAR_H_
