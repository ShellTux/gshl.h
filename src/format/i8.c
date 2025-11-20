#include "format/i8.h"
#include "format/isize.h"

#include "types/mod.h"

usize write_i8(char *buf, const i8 value, const usize precomputed_count)
{
    return write_isize(buf, value, precomputed_count);
}
