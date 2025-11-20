#include "format/i64.h"
#include "format/isize.h"

#include "types/mod.h"

usize write_i64(char *buf, const i64 value, const usize precomputed_count)
{
    return write_isize(buf, value, precomputed_count);
}
