#include "format/u64.h"
#include "format/usize.h"

#include "macros/mod.h"
#include "types/mod.h"

#include <assert.h>

usize write_u64(char *buf, GSHL_Template *t_mut)
{
    const GSHL_Template *const t = t_mut;

    GSHL_ASSERT(t->kind == GSHL_TEMPLATE_U64);

    return write_usize(buf, t_mut);
}
