// vim: foldmethod=marker
// gshl-priority: 81
#ifndef INCLUDE_FORMAT_WRAPPER_H_
#define INCLUDE_FORMAT_WRAPPER_H_

#include "types/mod.h"

#include <stdarg.h>
#include <stddef.h>

/// {{{ Macros

#define GSHL_FILL_TEMPLATE                                                     \
    /* TV(SPECIFIER, ENUM, NAME, TYPE, VALUE, OPTS) */                         \
    TV('%', CHAR, character, char, '%', {})                                    \
    /* T(SPECIFIER, ENUM, NAME, TYPE, VA_ARG, OPTS) */                         \
    T('b', BINARY, binary, u64, unsigned int, {})                              \
    T('B', BOOL, boolean, bool, unsigned int, {})                              \
    T('c', CHAR, character, char, int, {})                                     \
    T('d', I32, i32, i32, int, {})                                             \
    T('i', I32, i32, i32, int, {})                                             \
    T('f', F32, f32, f32, double, {})                                          \
    T('p', POINTER, pointer, void *, void *, {})                               \
    T('s', CSTRING, cstring, char *, char *, {})                               \
    T('u', U32, u32, u32, unsigned int, {})                                    \
    T('x', HEXADECIMAL, hex, hex, unsigned int, {.uppercase = false})          \
    T('X', HEXADECIMAL, hex, hex, unsigned int, {.uppercase = true})           \
    /* TL(SPECIFIER, ENUM, NAME, TYPE, VA_ARG_TYPE, OPTS) */                   \
    TL('b', BINARY, binary, u64, long unsigned, {})                            \
    TL('f', F64, f64, f64, double, {})                                         \
    TL('i', I64, i64, i64, long int, {})                                       \
    TL('u', U64, u64, u64, long unsigned, {})                                  \
    TL('x', HEXADECIMAL, hex, hex, long unsigned, {.uppercase = false})        \
    TL('X', HEXADECIMAL, hex, hex, long unsigned, {.uppercase = true})

#define GSHL_TEMPLATE_KINDS                                                    \
    /* KIND(ENUM, NAME, TYPE, VA_ARG_TYPE, BIT_SHIFT, OPTS, ...) */            \
    KIND(CSTRING, cstring, char *, char *, 0, {}, "cstring", "string", "s")    \
    KIND(CHAR, character, char, int, 1, {}, "character", "char", "c")          \
    KIND(                                                                      \
        POINTER, pointer, void *, void *, 2, { bool uppercase; }, "pointer",   \
        "p")                                                                   \
    KIND(                                                                      \
        HEXADECIMAL, hex, unsigned long, usize, 3, { bool uppercase; },        \
        "hexadecimal", "hex", "h")                                             \
    KIND(I8, i8, i8, int, 4, {}, "i8")                                         \
    KIND(I16, i16, i16, int, 5, {}, "i16")                                     \
    KIND(I32, i32, i32, int, 6, {}, "i32")                                     \
    KIND(I64, i64, i64, long int, 7, {}, "i64")                                \
    KIND(ISIZE, isize, isize, long int, 8, {}, "isize")                        \
    KIND(U8, u8, u8, unsigned int, 9, {}, "u8")                                \
    KIND(U16, u16, u16, unsigned int, 10, {}, "u16")                           \
    KIND(U32, u32, u32, unsigned int, 11, {}, "u32")                           \
    KIND(U64, u64, u64, unsigned long, 12, {}, "u64")                          \
    KIND(USIZE, usize, usize, unsigned long, 13, {}, "usize")                  \
    KIND(F32, f32, f32, double, 14, {}, "f32")                                 \
    KIND(F64, f64, f64, double, 15, {}, "f64")                                 \
    KIND(BOOL, boolean, bool, unsigned int, 16, {}, "boolean", "bool")         \
    KIND(BINARY, binary, u64, unsigned int, 17, {}, "binary", "bin")

/// }}}

/// {{{ Types

typedef struct GSHL_FormatWrapperOpts {
    char *prefix;
    char *suffix;
} GSHL_FormatWrapperOpts;

typedef enum GSHL_TemplateKind {
    GSHL_TEMPLATE_NONE = 0,
#define KIND(ENUM, NAME, TYPE, VA_ARG_TYPE, BIT_SHIFT, OPTS, ...)              \
    GSHL_TEMPLATE_##ENUM = 1 << BIT_SHIFT,
    GSHL_TEMPLATE_KINDS
#undef KIND
} GSHL_TemplateKind;

struct GSHL_Template;

typedef usize(GSHL_TemplateWriteFunction)(char *, struct GSHL_Template *);

typedef struct GSHL_Template {
    char *formatStart, *formatEnd;
    usize count;
    GSHL_TemplateKind kind;

    // Actual value of template
    union {
#define KIND(ENUM, NAME, TYPE, VA_ARG_TYPE, BIT_SHIFT, OPTS, ...) TYPE NAME;
        GSHL_TEMPLATE_KINDS
#undef KIND
    };

    // Options for this template
    union {
        struct GSHL_TemplateOptsNone {
            bool trash;
        } none;
#define KIND(ENUM, NAME, TYPE, VA_ARG_TYPE, BIT_SHIFT, OPTS, ...)              \
    struct GSHL_TemplateOpts_##NAME OPTS NAME;
        GSHL_TEMPLATE_KINDS
#undef KIND
    } opts;

    // Function to write value into buf
    union {
#define KIND(ENUM, NAME, TYPE, VA_ARG_TYPE, BIT_SHIFT, OPTS, ...)              \
    GSHL_TemplateWriteFunction *NAME;
        GSHL_TEMPLATE_KINDS
#undef KIND
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

GSHL_TemplateKind GSHL_template_kind_from_str(char **format);
GSHL_FormatWrapperResult
GSHL_format_wrapper(const char *restrict format,
                    const GSHL_FormatWrapperOpts formatOpts, ...);
GSHL_FormatWrapperResult
GSHL_format_wrapperv(const char *restrict format,
                     const GSHL_FormatWrapperOpts formatOpts, va_list args);
usize GSHL_format_wrapper_result_countv(const char *restrict format,
                                        const GSHL_FormatWrapperOpts opts,
                                        va_list args);
void GSHL_free_FormatWrapperResult(struct GSHL_FormatWrapperResult *result);
void *GSHL_format_memwrite(void **bufP, const void *src, const usize len);
void GSHL_format_register_debug(const char *restrict template,
                                usize (*write)(char *buf, const void *,
                                               const usize));
usize GSHL_format_count_templates(char **formatP);

/// }}}

#ifdef GSHL_STRIP_PREFIX
#    define format_wrapper GSHL_format_wrapper
#    define format_wrapperv GSHL_format_wrapperv
#    define free_FormatWrapperResult GHSL_free_FormatWrapperResult
#    define format_register_debug GSHL_format_register_debug
#    define format_memwrite GSHL_format_memwrite
#endif // GSHL_STRIP_PREFIX

#endif // INCLUDE_FORMAT_WRAPPER_H_
