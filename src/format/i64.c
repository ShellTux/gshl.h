#include "format/i64.h"
#include "format/isize.h"

#include "macros/mod.h"
#include "types/mod.h"

#include <assert.h>

usize write_i64(char *buf, GSHL_Template *t_mut)
{
    const GSHL_Template *const t = t_mut;

    GSHL_ASSERT(t->kind == GSHL_TEMPLATE_I64);

    return write_isize(buf, t_mut);
}
