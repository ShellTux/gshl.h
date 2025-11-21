#include "format/i64.h"
#include "format/isize.h"

#include "types/mod.h"

usize write_i64(char *buf, const i64 i64,
                const struct GSHL_TemplateOpts_i64 opts,
                const usize precomputed_count)
{
    const struct GSHL_TemplateOpts_isize new_opts = {};

    return write_isize(buf, i64, new_opts, precomputed_count);
}
