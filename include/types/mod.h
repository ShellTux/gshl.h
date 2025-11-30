// gshl-priority: 90
#ifndef INCLUDE_TYPES_MOD_H_
#define INCLUDE_TYPES_MOD_H_

#include "macros/mod.h"

#include <stdbool.h>
#include <stdint.h>

/// {{{ Macros

#define GSHL_TYPES                                                             \
    GSHL_TYPE(u8, uint8_t)                                                     \
    GSHL_TYPE(u16, uint16_t)                                                   \
    GSHL_TYPE(u32, uint32_t)                                                   \
    GSHL_TYPE(u64, uint64_t)                                                   \
    GSHL_TYPE(usize, uintmax_t)                                                \
    GSHL_TYPE(i8, int8_t)                                                      \
    GSHL_TYPE(i16, int16_t)                                                    \
    GSHL_TYPE(i32, int32_t)                                                    \
    GSHL_TYPE(i64, int64_t)                                                    \
    GSHL_TYPE(isize, intmax_t)                                                 \
    GSHL_TYPE(f32, float)                                                      \
    GSHL_TYPE(f64, double)                                                     \
    GSHL_TYPE(cstring, char *)                                                 \
    GSHL_TYPE(pointer, void *)                                                 \
    GSHL_TYPE(hex32, u32)                                                      \
    GSHL_TYPE(hex64, u64)                                                      \
    GSHL_TYPE(Fd, int)

/// }}}

#define GSHL_TYPE(ALIAS, TYPE) typedef TYPE ALIAS;
GSHL_TYPES
#undef GSHL_TYPE

#endif // INCLUDE_TYPES_MOD_H_
