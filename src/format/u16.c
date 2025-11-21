#include "format/u16.h"
#include "format/usize.h"

#include "types/mod.h"

usize write_u16(char *buf, const u16 u16,
                const struct GSHL_TemplateOpts_u16 opts,
                const usize precomputed_count)
{
    const struct GSHL_TemplateOpts_usize new_opts = {};

    return write_usize(buf, u16, new_opts, precomputed_count);
}
