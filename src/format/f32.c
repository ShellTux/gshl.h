#include "format/f32.h"

#include "macros/mod.h"
#include "types/mod.h"

#include <assert.h>
#include <stdlib.h>

usize write_f32(char *buf, GSHL_Template *t_mut)
{
    GSHL_UNUSED(buf);
    GSHL_UNUSED(t_mut);

    const GSHL_Template *const t = t_mut;

    GSHL_ASSERT(t->kind == GSHL_TEMPLATE_BINARY);

    GSHL_TODO("%s Not implemented yet", __FUNCTION__);

    return 0;
}
