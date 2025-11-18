#ifndef INCLUDE_TYPES_LIMITS_H_
#define INCLUDE_TYPES_LIMITS_H_

/// {{{ Macros

#define U8_MAX ((u8)((1U << 8) - 1))
#define U8_MIN 0

#define U16_MAX ((u16)((1U << 16) - 1))
#define U16_MIN 0

#define U32_MAX ((u32)((1UL << 32) - 1))
#define U32_MIN 0

#define U64_MAX ((u64)((1ULL << 64) - 1))
#define U64_MIN 0

#define USIZE_MAX ((usize)((1U << (sizeof(usize) * 8)) - 1))
#define USIZE_MIN 0

#define I8_MAX ((i8)((1 << 7) - 1))
#define I8_MIN ((i8)(-1 * (1 << 7)))

#define I16_MAX ((i16)((1 << 15) - 1))
#define I16_MIN ((i16)(-1 * (1 << 15)))

#define I32_MAX ((i32)((1U << 31) - 1))
#define I32_MIN ((i32)(-1 * (1U << 31)))

#define I64_MAX ((i64)((1ULL << 63) - 1))
#define I64_MIN ((i64)(-1 * (1ULL << 63)))

#define ISIZE_MAX ((isize)((1 << (sizeof(isize) * 8 - 1)) - 1))
#define ISIZE_MIN ((isize)(-1 * (1 << (sizeof(isize) * 8 - 1))))

/// }}}

#endif // INCLUDE_TYPES_LIMITS_H_
