#include "format/mod.h"
#include "format/usize.h"

#include "types/mod.h"

usize write_u8(char *buf, const u8 value,
               const struct GSHL_TemplateOpts_u8 opts,
               const usize precomputed_count)
{
    const struct GSHL_TemplateOpts_usize new_opts = {};

    return write_usize(buf, value, new_opts, precomputed_count);
}
