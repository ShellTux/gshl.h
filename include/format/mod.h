// vim: foldmethod=marker
#ifndef INCLUDE_INCLUDE_FORMAT_H_
#define INCLUDE_INCLUDE_FORMAT_H_

#include "types/mod.h"

#include <stdarg.h>
#include <stddef.h>

/// {{{ Types

typedef struct GSHL_FormatOpts {
} GSHL_FormatOpts;

typedef struct GSHL_Template {
    char *formatStart, *formatEnd;
    usize count;
    enum {
        GSHL_TEMPLATE_NONE = 0,
        GSHL_TEMPLATE_CSTRING,
        GSHL_TEMPLATE_CHAR,
        GSHL_TEMPLATE_POINTER,

        GSHL_TEMPLATE_HEXADECIMAL,

        GSHL_TEMPLATE_I8,
        GSHL_TEMPLATE_I16,
        GSHL_TEMPLATE_I32,
        GSHL_TEMPLATE_I64,
        GSHL_TEMPLATE_ISIZE,

        GSHL_TEMPLATE_U8,
        GSHL_TEMPLATE_U16,
        GSHL_TEMPLATE_U32,
        GSHL_TEMPLATE_U64,
        GSHL_TEMPLATE_USIZE,

        GSHL_TEMPLATE_F32,
        GSHL_TEMPLATE_F64,
    } kind;
    // Actual value of template
    union {
        i8 i8;
        i16 i16;
        i32 i32;
        i64 i64;
        isize isize;

        u8 u8;
        u16 u16;
        u32 u32;
        u64 u64;
        usize usize;

        f32 f32;
        f64 f64;

        char character;
        char *cstring;

        void *pointer;

        usize hex;
    };
    // Function to write value into buf
    union {
        usize (*character)(char *buf, const char character,
                           const usize precomputed_count);
        usize (*cstring)(char *buf, const char *cstring,
                         const usize precomputed_count);
        usize (*pointer)(char *buf, const void *pointer,
                         const usize precomputed_count);
        usize (*hex)(char *buf, const usize hex, const usize precomputed_count);

        usize (*i8)(char *buf, const i8 i8, const usize precomputed_count);
        usize (*i16)(char *buf, const i16 i16, const usize precomputed_count);
        usize (*i32)(char *buf, const i32 i32, const usize precomputed_count);
        usize (*i64)(char *buf, const i64 i64, const usize precomputed_count);
        usize (*isize)(char *buf, const isize isize,
                       const usize precomputed_count);

        usize (*u8)(char *buf, const u8 u8, const usize precomputed_count);
        usize (*u16)(char *buf, const u16 u16, const usize precomputed_count);
        usize (*u32)(char *buf, const u32 u32, const usize precomputed_count);
        usize (*u64)(char *buf, const u64 u64, const usize precomputed_count);
        usize (*usize)(char *buf, const usize usize,
                       const size_t precomputed_count);

        usize (*f32)(char *buf, const f32 f32, const usize precomputed_count);
        usize (*f64)(char *buf, const f64 f64, const usize precomputed_count);

    } write;
} GSHL_Template;

typedef struct GSHL_FormatWrapperResult {
    char *string;
    usize count;

    struct {
        struct GSHL_Template *items;
        usize count;
    } templates;
} GSHL_FormatWrapperResult;

/// }}}

/// {{{ Functions

GSHL_FormatWrapperResult GSHL_format_wrapper(const char *restrict format, ...);
GSHL_FormatWrapperResult GSHL_format_wrapperv(const char *restrict format,
                                              va_list args);
void GSHL_free_FormatWrapperResult(struct GSHL_FormatWrapperResult *result);

/// }}}

#ifdef GSHL_STRIP_PREFIX
#endif // GSHL_STRIP_PREFIX

#endif // INCLUDE_INCLUDE_FORMAT_H_
