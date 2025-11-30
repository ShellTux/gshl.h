// gshl-priority: 80
#ifndef INCLUDE_FORMAT_MOD_H_
#define INCLUDE_FORMAT_MOD_H_

#include "dynamic_array/mod.h"
#include "string/mod.h"

#include <stdarg.h>
#include <stddef.h>

/// {{{ Macros

#define GSHL_FORMAT_SPECIFIER_MAX_LEN 30
#define GSHL_FORMAT_SPEIFIERS_MAX_PER_TYPE 10

#define GSHL_FORMAT_SPECIFIERS                                                 \
    /* FS(BIT_SHIFT, ENUM, NAME, TYPE, OPTS, ...) */                           \
    FS(0, CSTRING, cstring, cstring, {}, "%s", "string", "cstring")            \
    FS(1, U8, u8, u8, {}, "u8")                                                \
    FS(2, U16, u16, u16, {}, "u16")                                            \
    FS(3, U32, u32, u32, {}, "%u", "u32")                                      \
    FS(4, U64, u64, u64, {}, "%lu", "u64")                                     \
    FS(5, USIZE, usize, usize, {}, "%lu", "usize")                             \
    FS(6, I8, i8, i8, {}, "i8")                                                \
    FS(7, I16, i16, i16, {}, "i16")                                            \
    FS(8, I32, i32, i32, {}, "%i", "%d", "i32")                                \
    FS(9, I64, i64, i64, {}, "%li", "%ld", "i64")                              \
    FS(10, ISIZE, isize, isize, {}, "%li", "%ld", "isize")                     \
    FS(11, CHAR, character, char, {}, "%c", "char", "character")               \
    FS(12, POINTER, pointer, pointer, { bool uppercase; }, "%p", "pointer")    \
    FS(13, HEX32, hex32, hex32, { bool uppercase; }, "%x", "%X", "hex32")      \
    FS(14, HEX64, hex64, hex64, { bool uppercase; }, "%lx", "%lX", "hex64")    \
    FS(15, BOOL, boolean, bool, {}, "%B", "boolean", "bool")

#define GSHL_FORMAT_SPECIFIER_REGISTER_ADDITIONAL(NAME, ...)                   \
    extern GSHL_FormatSpecifiers GSHL_additional_format_specifiers;            \
    void __attribute__((constructor)) GSHL_register_##NAME(void)               \
    {                                                                          \
        const GSHL_FormatSpecifier fs = {__VA_ARGS__};                         \
                                                                               \
        GSHL_DArray_append(&GSHL_additional_format_specifiers, fs);            \
    }

/// }}}

/// {{{ Types

GSHL_DArrayTypeDecl(GSHL_FormatString, char);

typedef enum GSHL_FormatSpecifierKind {
    GSHL_FORMAT_SPECIFIER_NONE = 0,
#define FS(BIT_SHIFT, ENUM, NAME, TYPE, OPTS, ...)                             \
    GSHL_FORMAT_SPECIFIER_##ENUM = 1 << BIT_SHIFT,
    GSHL_FORMAT_SPECIFIERS
#undef FS
} GSHL_FormatSpecifierKind;

typedef union GSHL_FormatSpecifierOpts {
#define FS(BIT_SHIFT, ENUM, NAME, TYPE, OPTS, ...)                             \
    struct GSHL_FormatSpecifierOpts_##TYPE OPTS NAME;
    GSHL_FORMAT_SPECIFIERS
#undef FS
} GSHL_FormatSpecifierOpts;

typedef union GSHL_FormatSpecifierValue {
#define FS(BIT_SHIFT, ENUM, NAME, TYPE, OPTS, ...) TYPE NAME;
    GSHL_FORMAT_SPECIFIERS
#undef FS
} GSHL_FormatSpecifierValue;

typedef struct GSHL_FormatSpecifier GSHL_FormatSpecifier;

typedef usize(GSHL_FormatSpecifierWriteFunction)(GSHL_FormatString *,
                                                 const GSHL_FormatSpecifier *);

struct GSHL_FormatSpecifier {
    GSHL_FormatSpecifierKind kind;
    GSHL_FormatSpecifierWriteFunction *write;
    GSHL_FormatSpecifierValue value;
    GSHL_FormatSpecifierOpts opts;
    usize va_size;
    char specifiers[GSHL_FORMAT_SPEIFIERS_MAX_PER_TYPE]
                   [GSHL_FORMAT_SPECIFIER_MAX_LEN];
};

GSHL_DArrayTypeDecl(GSHL_FormatSpecifiers, GSHL_FormatSpecifier);

/// }}}

/// {{{ Functions

GSHL_StringView GSHL_format_wrapper(const char *const restrict format, ...);
GSHL_StringView GSHL_format_wrapperv(const char *const restrict format,
                                     va_list args);

usize GSHL_format_write(GSHL_FormatString *string,
                        const char *const restrict format, ...);
usize GSHL_format_writev(GSHL_FormatString *string,
                         const char *const restrict format, va_list args);

char *GSHL_format(const char *const restrict format, ...);
char *GSHL_formatv(const char *const restrict format, va_list args);

void GSHL_print_registered_format_specifiers(void);

bool GSHL_format_specifier_register(const GSHL_FormatSpecifier fs);

/// }}}

#ifdef GSHL_STRIP_PREFIX
#    define format GSHL_format
#    define formatv GSHL_formatv
#    define print_registered_format_specifiers                                 \
        GSHL_print_registered_format_specifiers
#    define format_write GSHL_format_write
#    define format_writev GSHL_format_writev
#    define FormatSpecifier GSHL_FormatSpecifier
#    define FormatString GSHL_FormatString
#    define format_specifier_register GSHL_format_specifier_register
#    define FORMAT_SPECIFIER_REGISTER_ADDITIONAL                               \
        GSHL_FORMAT_SPECIFIER_REGISTER_ADDITIONAL
#endif

#endif // INCLUDE_FORMAT_MOD_H_
