#include "format/u64.h"
#include "format/usize.h"

#include "types/mod.h"

usize write_u64(char *buf, const u64 value, const usize precomputed_count)
{
    return write_usize(buf, value, precomputed_count);
}
