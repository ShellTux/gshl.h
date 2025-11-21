#include "format/u32.h"
#include "format/usize.h"

#include "types/mod.h"

usize write_u32(char *buf, const u32 u32,
                const struct GSHL_TemplateOpts_u32 opts,
                const usize precomputed_count)
{
    const struct GSHL_TemplateOpts_usize new_opts = {};

    return write_usize(buf, u32, new_opts, precomputed_count);
}
