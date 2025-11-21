#include "format/i32.h"
#include "format/isize.h"

#include "types/mod.h"

usize write_i32(char *buf, const i32 i32,
                const struct GSHL_TemplateOpts_i32 opts,
                const usize precomputed_count)
{
    const struct GSHL_TemplateOpts_isize new_opts = {};

    return write_isize(buf, i32, new_opts, precomputed_count);
}
