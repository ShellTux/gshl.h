#ifndef INCLUDE_OPTION_MOD_H_
#define INCLUDE_OPTION_MOD_H_

#include "types/mod.h"

#define Option(TYPE) Option_##TYPE
#define None(TYPE)                                                             \
    (Option_##TYPE) { .variant = GSHL_NoneTag }
#define Some(TYPE, VALUE)                                                      \
    (Option_##TYPE) { .variant = GSHL_SomeTag, .some.value = VALUE, }

#define OptionDecl(TYPE)                                                       \
    typedef struct Some_##TYPE Some_##TYPE;                                    \
    typedef struct Option_##TYPE {                                             \
        enum OptionVariants variant;                                           \
        union {                                                                \
            struct Some_##TYPE {                                               \
                TYPE value;                                                    \
            } some;                                                            \
        };                                                                     \
    } Option_##TYPE;                                                           \
    usize GSHL_write_Option_##TYPE(GSHL_FormatString *string,                  \
                                   const GSHL_FormatSpecifier *const fs)       \
    {                                                                          \
        const Option(TYPE) *opt = fs->value.pointer;                           \
        switch (opt->variant) {                                                \
        case GSHL_NoneTag:                                                     \
            return GSHL_format_write(string, "Option::" #TYPE "(None)");       \
        case GSHL_SomeTag:                                                     \
            return GSHL_format_write(string,                                   \
                                     "Option::" #TYPE "(Some({" #TYPE "}))",   \
                                     opt->some.value);                         \
        default:                                                               \
            GSHL_UNREACHABLE("Unknown variant: %u", opt->variant);             \
            break;                                                             \
        }                                                                      \
                                                                               \
        return 0;                                                              \
    }                                                                          \
                                                                               \
    GSHL_FORMAT_SPECIFIER_REGISTER(                                            \
        GSHL_Option_##TYPE, .kind = GSHL_FORMAT_SPECIFIER_POINTER,             \
        .va_size = sizeof(Option_##TYPE *), .write = GSHL_write_Option_##TYPE, \
        .specifiers = {"Option_" #TYPE, "Option(" #TYPE ")",                   \
                       "Option::" #TYPE})

#define is_some() variant == GSHL_SomeTag
#define is_none() variant == GSHL_NoneTag

#define Option_unwrap(A)                                                       \
    (Option_is_some_or_panic((A).variant,                                      \
                             "%s:%i: Panic: unwrap on a None value", __FILE__, \
                             __LINE__)                                         \
         ? (A).some.value                                                      \
         : (A).some.value)

#define Option_unwrap_or(A, OR_VALUE)                                          \
    (((A).is_some()) ? (A).some.value : OR_VALUE)

/// {{{ Types

typedef enum OptionVariants { GSHL_NoneTag, GSHL_SomeTag } OptionVariants;

/// }}}

/// {{{ Functions

bool Option_is_some_or_panic(const enum OptionVariants variant,
                             const char *const restrict format, ...);

/// }}}

#endif // INCLUDE_OPTION_MOD_H_
