#include "format/u32.h"
#include "format/usize.h"

#include "macros/mod.h"
#include "types/mod.h"

#include <assert.h>

usize write_u32(char *buf, GSHL_Template *t_mut)
{
    const GSHL_Template *const t = t_mut;

    GSHL_ASSERT(t->kind == GSHL_TEMPLATE_U32);

    return write_usize(buf, t_mut);
}
