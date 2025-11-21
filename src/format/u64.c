#include "format/u64.h"
#include "format/usize.h"

#include "types/mod.h"

usize write_u64(char *buf, const u64 value,
                const struct GSHL_TemplateOpts_u64 opts,
                const usize precomputed_count)
{
    const struct GSHL_TemplateOpts_usize new_opts = {};

    return write_usize(buf, value, new_opts, precomputed_count);
}
