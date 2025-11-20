#include "format/u8.h"
#include "format/usize.h"

#include "types/mod.h"

usize write_u8(char *buf, const u8 value, const usize precomputed_count)
{
    return write_usize(buf, value, precomputed_count);
}
