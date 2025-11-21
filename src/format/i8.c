#include "format/i8.h"
#include "format/isize.h"

#include "types/mod.h"

usize write_i8(char *buf, const i8 i8, const struct GSHL_TemplateOpts_i8 opts,
               const usize precomputed_count)
{
    const struct GSHL_TemplateOpts_isize new_opts = {};

    return write_isize(buf, i8, new_opts, precomputed_count);
}
