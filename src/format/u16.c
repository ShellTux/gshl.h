#include "format/u16.h"
#include "format/usize.h"

#include "types/mod.h"

usize write_u16(char *buf, const u16 value, const usize precomputed_count)
{
    return write_usize(buf, value, precomputed_count);
}
