#ifndef INCLUDE_GSHL_H_
#define INCLUDE_GSHL_H_
// gshl-priority: 99

#include <stdio.h> // IWYU pragma: keep

#ifndef GSHLDEF
#    define GSHLDEF
#endif

#define GSHL_UNUSED(X) (void)(X)
#define GSHL_EXIT(...)                                                         \
    do {                                                                       \
        fprintf(stderr, "%s:%d: ", __FILE__, __LINE__);                        \
        fprintf(stderr, __VA_ARGS__);                                          \
        fprintf(stderr, "\n");                                                 \
        exit(1);                                                               \
    } while (0)
#define GSHL_TODO(...) GSHL_EXIT("TODO: " __VA_ARGS__)
#define GSHL_UNREACHABLE(...) GSHL_EXIT("UNREACHABLE: " __VA_ARGS__)
#define GSHL_LOOP for (;;)
#ifdef GSHL_DEBUG
#    define GSHL_ASSERT(...) assert(__VA_ARGS__)
#else
#    define GSHL_ASSERT(...) GSHL_UNUSED((__VA_ARGS__))
#endif

#ifdef GSHL_STRIP_PREFIX
#    define ASSERT GSHL_ASSERT
#    define loop GSHL_LOOP
#    define LOOP GSHL_LOOP
#    define TODO GSHL_TODO
#    define UNREACHABLE GSHL_UNREACHABLE
#    define UNUSED GSHL_UNUSED
#endif

// gshl-priority: 90

#include <stdbool.h>
#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef uintmax_t usize;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef intmax_t isize;

typedef float f32;
typedef double f64;

// vim: foldmethod=marker
// gshl-priority: 81


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
    usize (*NAME)(char *, struct GSHL_Template *);
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

// gshl-priority: 80


#include <stdarg.h>

/// {{{ Types

typedef struct GSHL_PrintWrapperOpts {
    usize fd;
} GSHL_PrintWrapperOpts;

/// }}}

/// {{{ Functions

struct GSHL_FormatWrapperOpts;
usize GSHL_print_wrapperv(const GSHL_PrintWrapperOpts print_opts,
                          const struct GSHL_FormatWrapperOpts format_opts,
                          const char *restrict format, va_list args);

usize GSHL_print(const char *restrict format, ...);
usize GSHL_dprint(const int fd, const char *restrict format, ...);
usize GSHL_eprint(const char *restrict format, ...);
usize GSHL_println(const char *restrict format, ...);
usize GSHL_dprintln(const int fd, const char *restrict format, ...);
usize GSHL_eprintln(const char *restrict format, ...);

/// }}}

#ifdef GSHL_STRIP_PREFIX
#    define print GSHL_print
#    define dprint GSHL_dprint
#    define println GSHL_println
#    define dprintln GSHL_dprintln
#endif


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

#define F32_MAX 340282346638528859811704183484516925440.0000000000000000f
#define F32_MIN -340282346638528859811704183484516925440.0000000000000000f

#define F64_MAX                                                                \
    179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.0000000000000000
#define F64_MIN                                                                \
    -179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.0000000000000000

/// }}}

// vim: foldmethod=marker

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>  // IWYU pragma: keep
#include <string.h> // IWYU pragma: keep


/// {{{ Macros

#define GSHL_TEST(FUNC)                                                        \
    GSHL_TestFunction test_##FUNC;                                             \
    __attribute__((constructor)) void register_test_##FUNC(void)               \
    {                                                                          \
        GSHL_register_test_wrapper(#FUNC, test_##FUNC);                        \
    }                                                                          \
    void test_##FUNC(usize *failed_tests)

#define GSHL_REGISTER_TEST(TEST_FUNC)                                          \
    GSHL_register_test_wrapper(#TEST_FUNC, TEST_FUNC)

#define GSHL_TEST_EQUAL(EXPR1, EXPR2, ...)                                     \
    do {                                                                       \
        GSHL_TestEqualOpt opt = (GSHL_TestEqualOpt){__VA_ARGS__};              \
                                                                               \
        if ((EXPR1) != (EXPR2)) {                                              \
            *failed_tests += 1;                                                \
            fprintf(stderr,                                                    \
                    " %s:%02d " GSHL_FG_CYAN(" %-30s ") " ... " GSHL_FG_RED(   \
                        " FAILED ") " Assertion failed : %s == %s\n",          \
                    __FILE__, __LINE__, __func__, #EXPR1, #EXPR2);             \
                                                                               \
            if (opt.continue_on_fail) {                                        \
                return;                                                        \
            }                                                                  \
        }                                                                      \
        else {                                                                 \
            fprintf(stderr,                                                    \
                    " %s:%02d " GSHL_FG_CYAN(" %-30s ") " ... " GSHL_FG_GREEN( \
                        " PASSED ") "\n",                                      \
                    __FILE__, __LINE__, __func__);                             \
        }                                                                      \
    } while (0)

#define GSHL_TEST_STR_EQUAL(STR1, STR2, ...)                                   \
    do {                                                                       \
        GSHL_TestEqualOpt opt = (GSHL_TestEqualOpt){__VA_ARGS__};              \
                                                                               \
        if (strcmp((STR1), (STR2)) != 0) {                                     \
            *failed_tests += 1;                                                \
            if (opt.align_on_new_line) {                                       \
                fprintf(                                                       \
                    stderr,                                                    \
                    " %s:%02d " GSHL_FG_CYAN(" %-30s ") " ... " GSHL_FG_RED(   \
                        " FAILED ") " Assertion failed : \n\"%s\" == "         \
                                    "\n\"%s\"\n",                              \
                    __FILE__, __LINE__, __func__, STR1, STR2);                 \
            }                                                                  \
            else {                                                             \
                fprintf(                                                       \
                    stderr,                                                    \
                    " %s:%02d " GSHL_FG_CYAN(" %-30s ") " ... " GSHL_FG_RED(   \
                        " FAILED ") " Assertion failed : \"%s\" == \"%s\"\n",  \
                    __FILE__, __LINE__, __func__, STR1, STR2);                 \
            }                                                                  \
                                                                               \
            if (opt.continue_on_fail) {                                        \
                return;                                                        \
            }                                                                  \
        }                                                                      \
        else {                                                                 \
            fprintf(stderr,                                                    \
                    " %s:%02d " GSHL_FG_CYAN(" %-30s ") " ... " GSHL_FG_GREEN( \
                        " PASSED ") "\n",                                      \
                    __FILE__, __LINE__, __func__);                             \
        }                                                                      \
    } while (0)

/// }}}

/// {{{ Types

typedef void(GSHL_TestFunction)(usize *failed_tests);

typedef struct GSHL_Test {
    char *name;
    GSHL_TestFunction *func;
    struct GSHL_Test *next;
    struct GSHL_Test *prev;
} GSHL_Test;

typedef struct GSHL_TestEqualOpt {
    bool continue_on_fail;
    bool align_on_new_line;
} GSHL_TestEqualOpt;

/// }}}

/// {{{ Functions

GSHLDEF void GSHL_register_test_wrapper(const char *name,
                                        GSHL_TestFunction test_func);
GSHLDEF usize GSHL_run_tests(const char *filter);

/// }}}

#ifdef GSHL_STRIP_PREFIX
#    define TEST_EQUAL GSHL_TEST_EQUAL
#    define REGISTER_TEST GSHL_REGISTER_TEST
#    define run_tests GSHL_run_tests
#endif



/// {{{ Macros

#define GSHL_STACK_STRING_LEN(ARRAY) (sizeof(ARRAY) - 1)

/// }}}

/// {{{ Types

typedef struct GSHL_StringView {
    char *start;
    usize len;
} GSHL_StringView;

/// }}}

#ifdef GSHL_TESTS
#    define STACK_STRING_LEN GSHL_STACK_STRING_LEN
#    define StringView GSHL_StringView
#endif



/// {{{ Functions

usize GSHL_factorial(const usize number);

/// }}}

#ifdef GSHL_STRIP_PREFIX
#    define factorial GSHL_factorial
#endif



/// {{{ Functions

usize write_usize(char *buf, GSHL_Template *template);

/// }}}



/// {{{ Functions

usize write_u8(char *buf, GSHL_Template *template);

/// }}}



/// {{{ Functions

usize write_u64(char *buf, GSHL_Template *template);

/// }}}



/// {{{ Functions

usize write_u32(char *buf, GSHL_Template *template);

/// }}}



/// {{{ Functions

usize write_u16(char *buf, GSHL_Template *template);

/// }}}



/// {{{ Functions

usize write_pointer(char *buf, GSHL_Template *template);

/// }}}



#include <stdarg.h>

/// {{{ Functions

char *GSHL_format(const char *restrict format, ...);
char *GSHL_formatv(const char *restrict format, va_list args);
char *GSHL_formatln(const char *restrict format, ...);

/// }}}

#ifdef GSHL_STRIP_PREFIX
#    define format GSHL_format
#endif



/// {{{ Functions

usize write_isize(char *buf, GSHL_Template *template);

/// }}}



/// {{{ Functions

usize write_i8(char *buf, GSHL_Template *template);

/// }}}



/// {{{ Functions

usize write_i64(char *buf, GSHL_Template *template);

/// }}}



/// {{{ Functions

usize write_i32(char *buf, GSHL_Template *template);

/// }}}



/// {{{ Functions

usize write_i16(char *buf, GSHL_Template *template);

/// }}}



/// {{{ Functions

usize write_hex(char *buf, GSHL_Template *template);

/// }}}



/// {{{ Functions

usize write_f64(char *buf, GSHL_Template *template);

/// }}}



/// {{{ Functions

usize write_f32(char *buf, GSHL_Template *template);

/// }}}



/// {{{ Functions

usize write_cstring(char *buf, GSHL_Template *template);

/// }}}



/// {{{ Functions

usize write_character(char *buf, GSHL_Template *template);

/// }}}



/// {{{ Functions

usize write_boolean(char *buf, GSHL_Template *template);

/// }}}



/// {{{ Functions

usize write_binary(char *buf, GSHL_Template *template);

/// }}}



/// {{{ Macros

#define GSHL_ARRAYN_PRINT(ARRAY, N, ELEMENT_FORMAT, ...)                       \
    do {                                                                       \
        struct GSHL_ArrayPrintOpts opts = {__VA_ARGS__};                       \
        opts.sep = opts.sep ? opts.sep : ", ";                                 \
        opts.end = opts.end ? opts.end : "\n";                                 \
        opts.array_limits = opts.array_limits ? opts.array_limits : "{}";      \
        printf("%s[%zu] = %c ", #ARRAY, N, opts.array_limits[0]);              \
        for (usize array_index = 0; array_index < N; ++array_index) {          \
            if (array_index == 0) {                                            \
                printf(ELEMENT_FORMAT, ARRAY[array_index]);                    \
            }                                                                  \
            else {                                                             \
                printf("%s" ELEMENT_FORMAT, opts.sep, ARRAY[array_index]);     \
            }                                                                  \
        }                                                                      \
        printf(" %c%s", opts.array_limits[1], opts.end);                       \
    } while (0)
#define GSHL_ARRAYN_FOREACH(ARRAY, N, VARDECL, ...)                            \
    for (usize keep = 1, index = 0, size = (N); keep && index < size;          \
         keep = !keep, index++)                                                \
        for (const VARDECL = (ARRAY)[index]; keep; keep = !keep)
#define GSHL_ARRAYN_FOREACH_MUT(ARRAY, N, VARDECL)                             \
    for (usize keep = 1, index = 0, size = (N); keep && index < size;          \
         keep = !keep, index++)                                                \
        for (VARDECL = &((ARRAY)[index]); keep; keep = !keep)

#define GSHL_ARRAY_LEN(ARRAY) (sizeof(ARRAY) / sizeof(ARRAY[0]))
#define GSHL_ARRAY_PRINT(ARRAY, ELEMENT_FORMAT, ...)                           \
    GSHL_ARRAYN_PRINT(ARRAY, GSHL_ARRAY_LEN(ARRAY), ELEMENT_FORMAT, __VA_ARGS__)
#define GSHL_ARRAY_FOREACH(ARRAY, VARDECL, ...)                                \
    GSHL_ARRAYN_FOREACH(ARRAY, GSHL_ARRAY_LEN(ARRAY), VARDECL, __VA_ARGS__)
#define GSHL_ARRAY_FOREACH_MUT(ARRAY, VARDECL)                                 \
    GSHL_ARRAYN_FOREACH_MUT(ARRAY, GSHL_ARRAY_LEN(ARRAY), VARDECL)

/// }}}

/// {{{ Types

typedef struct GSHL_ArrayPrintOpts {
    char *sep;
    char *end;
    char *array_limits;
} GSHL_ArrayPrintOpts;

typedef struct GSHL_ArrayForEachOpts {
    usize length;

    u8 unused[];
} GSHL_ArrayForEachOpts;

/// }}}

/// {{{ Functions

/// }}}

/// {{{ Strip Prefix

#ifdef GSHL_STRIP_PREFIX
#    define ARRAY_LEN GSHL_ARRAY_LEN
#    define ARRAY_PRINT GSHL_ARRAY_PRINT
#    define ARRAY_FOREACH GSHL_ARRAY_FOREACH
#    define ARRAY_FOREACH_MUT GSHL_ARRAY_FOREACH_MUT
#    define ARRAYN_PRINT GSHL_ARRAYN_PRINT
#    define ARRAYN_FOREACH GSHL_ARRAYN_FOREACH
#    define ARRAYN_FOREACH_MUT GSHL_ARRAYN_FOREACH_MUT
#endif

/// }}}




/// {{{ Macros

#define GSHL_FG_BLACK(TEXT) "\033[30m" TEXT "\033[0m"
#define GSHL_FG_RED(TEXT) "\033[31m" TEXT "\033[0m"
#define GSHL_FG_GREEN(TEXT) "\033[32m" TEXT "\033[0m"
#define GSHL_FG_YELLOW(TEXT) "\033[33m" TEXT "\033[0m"
#define GSHL_FG_BLUE(TEXT) "\033[34m" TEXT "\033[0m"
#define GSHL_FG_MAGENTA(TEXT) "\033[35m" TEXT "\033[0m"
#define GSHL_FG_CYAN(TEXT) "\033[36m" TEXT "\033[0m"
#define GSHL_FG_WHITE(TEXT) "\033[37m" TEXT "\033[0m"

#define GSHL_BG_BLACK(TEXT) "\033[40m" TEXT "\033[0m"
#define GSHL_BG_RED(TEXT) "\033[41m" TEXT "\033[0m"
#define GSHL_BG_GREEN(TEXT) "\033[42m" TEXT "\033[0m"
#define GSHL_BG_YELLOW(TEXT) "\033[43m" TEXT "\033[0m"
#define GSHL_BG_BLUE(TEXT) "\033[44m" TEXT "\033[0m"
#define GSHL_BG_MAGENTA(TEXT) "\033[45m" TEXT "\033[0m"
#define GSHL_BG_CYAN(TEXT) "\033[46m" TEXT "\033[0m"
#define GSHL_BG_WHITE(TEXT) "\033[47m" TEXT "\033[0m"

/// }}}

/// {{{ Strip Prefix

#ifdef GSHL_STRIP_PREFIX
#    define FG_BLACK(...) GSHL_FG_BLACK(__VA_ARGS__)
#    define FG_RED(...) GSHL_FG_RED(__VA_ARGS__)
#    define FG_GREEN(...) GSHL_FG_GREEN(__VA_ARGS__)
#    define FG_YELLOW(...) GSHL_FG_YELLOW(__VA_ARGS__)
#    define FG_BLUE(...) GSHL_FG_BLUE(__VA_ARGS__)
#    define FG_MAGENTA(...) GSHL_FG_MAGENTA(__VA_ARGS__)
#    define FG_CYAN(...) GSHL_FG_CYAN(__VA_ARGS__)
#    define FG_WHITE(...) GSHL_FG_WHITE(__VA_ARGS__)

#    define BG_BLACK(...) GSHL_BG_BLACK(__VA_ARGS__)
#    define BG_RED(...) GSHL_BG_RED(__VA_ARGS__)
#    define BG_GREEN(...) GSHL_BG_GREEN(__VA_ARGS__)
#    define BG_YELLOW(...) GSHL_BG_YELLOW(__VA_ARGS__)
#    define BG_BLUE(...) GSHL_BG_BLUE(__VA_ARGS__)
#    define BG_MAGENTA(...) GSHL_BG_MAGENTA(__VA_ARGS__)
#    define BG_CYAN(...) GSHL_BG_CYAN(__VA_ARGS__)
#    define BG_WHITE(...) GSHL_BG_WHITE(__VA_ARGS__)
#endif

/// }}}

#endif // INCLUDE_GSHL_H_
#ifdef GSHL_IMPLEMENTATION
#ifdef GSHL_SOURCE_CODE_MAPPING
#    line 2 "src/types/mod.c"
#endif // GSHL_SOURCE_CODE_MAPPING

#ifdef GSHL_TESTS


GSHL_TEST(types_size)
{
    GSHL_TEST_EQUAL(sizeof(u8), 1);
    GSHL_TEST_EQUAL(sizeof(u16), 2);
    GSHL_TEST_EQUAL(sizeof(u32), 4);
    GSHL_TEST_EQUAL(sizeof(u64), 8);
    GSHL_TEST_EQUAL(sizeof(usize), 8);

    GSHL_TEST_EQUAL(sizeof(i8), 1);
    GSHL_TEST_EQUAL(sizeof(i16), 2);
    GSHL_TEST_EQUAL(sizeof(i32), 4);
    GSHL_TEST_EQUAL(sizeof(i64), 8);
    GSHL_TEST_EQUAL(sizeof(isize), 8);

    GSHL_TEST_EQUAL(sizeof(bool), 1);

    GSHL_TEST_EQUAL(sizeof(f32), 4);
    GSHL_TEST_EQUAL(sizeof(f64), 8);
}

#endif
#ifdef GSHL_SOURCE_CODE_MAPPING
#    line 2 "src/types/limits.c"
#endif // GSHL_SOURCE_CODE_MAPPING

#ifdef GSHL_TESTS

GSHL_TEST(types_limits)
{
    GSHL_TEST_EQUAL((u8)(U8_MAX + 1), 0);
    GSHL_TEST_EQUAL((u8)(U8_MIN - 1), U8_MAX);

    GSHL_TEST_EQUAL((u16)(U16_MAX + 1), 0);
    GSHL_TEST_EQUAL((u16)(U16_MIN - 1), U16_MAX);

    GSHL_TEST_EQUAL((u32)(U32_MAX + 1), 0);
    GSHL_TEST_EQUAL((u32)(U32_MIN - 1), U32_MAX);
}

#endif
#ifdef GSHL_SOURCE_CODE_MAPPING
#    line 4 "src/test/mod.c"
#endif // GSHL_SOURCE_CODE_MAPPING

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static GSHL_Test *GSHL_test_list = NULL;

GSHLDEF usize GSHL_run_tests(const char *filter)
{
    usize failed_tests = 0;
    size_t total = 0;

    printf("Running tests...\n");

    // TODO: Remove O(n)
    GSHL_Test *last = NULL;
    for (last = GSHL_test_list; last != NULL && last->next != NULL;
         last = last->next) {
    }

    /* Reverse Order */
    for (GSHL_Test *current = last; current != NULL; current = current->prev) {
        if (filter != NULL && strstr(current->name, filter) == NULL) {
            continue;
        }

        current->func(&failed_tests);
        total++;
    }

    printf("\n");
    if (failed_tests != 0) {
        printf(GSHL_FG_RED("Failed tests:") " %zu\n", failed_tests);
    }
    printf("Total Tests:  %zu\n", total);

    return failed_tests;
}

GSHLDEF void GSHL_register_test_wrapper(const char *name,
                                        GSHL_TestFunction test_func)
{
    GSHL_Test *new_test = malloc(sizeof(*new_test));

    if (GSHL_test_list != NULL) {
        GSHL_test_list->prev = new_test;
    }

    new_test->name = (char *)name;
    new_test->func = test_func;
    new_test->next = GSHL_test_list;
    new_test->prev = NULL;

    GSHL_test_list = new_test;
}

int test_main(int argc, char *argv[])
{
    GSHL_UNUSED(argc);
    GSHL_UNUSED(argv);

    GSHL_run_tests(NULL);

    return 0;
}
#ifdef GSHL_SOURCE_CODE_MAPPING
#    line 3 "src/print/mod.c"
#endif // GSHL_SOURCE_CODE_MAPPING

#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>

static const struct GSHL_PrintWrapperOpts defPrintOpts = {.fd = STDOUT_FILENO};

usize GSHL_print_wrapperv(const GSHL_PrintWrapperOpts print_opts,
                          const GSHL_FormatWrapperOpts format_opts,
                          const char *restrict format, va_list args)
{
    struct GSHL_FormatWrapperResult result =
        GSHL_format_wrapperv(format, format_opts, args);

    if (result.templates.items != NULL) {
        free(result.templates.items);
        result.templates.items = NULL;
        result.templates.count = 0;
    }

    const usize count = write(print_opts.fd, result.string, result.count);

    GSHL_free_FormatWrapperResult(&result);

    return count;
}

usize GSHL_print(const char *restrict format, ...)
{
    static const GSHL_PrintWrapperOpts print_opts = {.fd = STDOUT_FILENO};
    static const GSHL_FormatWrapperOpts format_opts = {};

    va_list args;
    va_start(args, format);
    const usize count =
        GSHL_print_wrapperv(print_opts, format_opts, format, args);
    va_end(args);

    return count;
}

usize GSHL_dprint(const int fd, const char *restrict format, ...)
{
    const GSHL_PrintWrapperOpts print_opts = {.fd = fd};
    static const GSHL_FormatWrapperOpts format_opts = {};

    va_list args;
    va_start(args, format);
    const usize count =
        GSHL_print_wrapperv(print_opts, format_opts, format, args);
    va_end(args);

    return count;
}

usize GSHL_eprint(const char *restrict format, ...)
{
    static const GSHL_PrintWrapperOpts print_opts = {.fd = STDERR_FILENO};
    static const GSHL_FormatWrapperOpts format_opts = {};

    va_list args;
    va_start(args, format);
    const usize count =
        GSHL_print_wrapperv(print_opts, format_opts, format, args);
    va_end(args);

    return count;
}

usize GSHL_println(const char *restrict format, ...)
{
    static const GSHL_FormatWrapperOpts format_opts = {.suffix = "\n"};
    static const GSHL_PrintWrapperOpts print_opts = {.fd = STDOUT_FILENO};

    va_list args;
    va_start(args, format);
    const usize count =
        GSHL_print_wrapperv(print_opts, format_opts, format, args);
    va_end(args);

    return count;
}

usize GSHL_dprintln(const int fd, const char *restrict format, ...)
{
    static const GSHL_FormatWrapperOpts format_opts = {.suffix = "\n"};
    const GSHL_PrintWrapperOpts print_opts = {.fd = fd};

    va_list args;
    va_start(args, format);
    const usize count =
        GSHL_print_wrapperv(print_opts, format_opts, format, args);
    va_end(args);

    return count;
}

usize GSHL_eprintln(const char *restrict format, ...)

{
    static const GSHL_FormatWrapperOpts format_opts = {.suffix = "\n"};
    static const GSHL_PrintWrapperOpts print_opts = {.fd = STDERR_FILENO};

    va_list args;
    va_start(args, format);
    const usize count =
        GSHL_print_wrapperv(print_opts, format_opts, format, args);
    va_end(args);

    return count;
}
#ifdef GSHL_SOURCE_CODE_MAPPING
#    line 2 "src/math/mod.c"
#endif // GSHL_SOURCE_CODE_MAPPING

usize GSHL_factorial(const usize number)
{
    usize result = 1;
    for (usize n = 2; n <= number; ++n) {
        result *= n;
    }

    return result;
}

#ifdef GSHL_TESTS


GSHL_TEST(factorial)
{
    GSHL_TEST_EQUAL(GSHL_factorial(0), 1);
    GSHL_TEST_EQUAL(GSHL_factorial(1), 1);
    GSHL_TEST_EQUAL(GSHL_factorial(2), 2);
    GSHL_TEST_EQUAL(GSHL_factorial(3), 6);
    GSHL_TEST_EQUAL(GSHL_factorial(4), 24);
    GSHL_TEST_EQUAL(GSHL_factorial(5), 120);
    GSHL_TEST_EQUAL(GSHL_factorial(6), 720);
    GSHL_TEST_EQUAL(GSHL_factorial(7), 5040);
}

#endif
#ifdef GSHL_SOURCE_CODE_MAPPING
#    line 2 "src/format/wrapper.c"
#endif // GSHL_SOURCE_CODE_MAPPING



#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#define GSHL_FORMAT_DEBUG 0

#if GSHL_FORMAT_DEBUG > 0
#    include <stdio.h>
#endif

#if GHSL_FORMAT_DEBUG == 10
#    define GSHL_FORMAT_DEBUG_FUNC printf("%s\n", __FUNCTION__)
#    define GSHL_FORMAT_DEBUG_PRINT(...) printf(__VA_ARGS__)
#elif GSHL_FORMAT_DEBUG == 2
#    define GSHL_FORMAT_DEBUG_FUNC
#    define GSHL_FORMAT_DEBUG_PRINT(...) printf(__VA_ARGS__)
#elif GSHL_FORMAT_DEBUG == 1
#    define GSHL_FORMAT_DEBUG_FUNC printf("%s\n", __FUNCTION__)
#    define GSHL_FORMAT_DEBUG_PRINT(...) printf(__VA_ARGS__)
#elif GSHL_FORMAT_DEBUG == 0
#    define GSHL_FORMAT_DEBUG_FUNC
#    define GSHL_FORMAT_DEBUG_PRINT(...)
#endif

/*
 * This function returns GSHL_TemplateKind based on the format string
 * It will stop at the first ';', '}' or '\0'
 * It will modify in place the format to advance the *format to the ';' or '}'
 */
GSHL_TemplateKind GSHL_template_kind_from_str(char **format)
{
    static const struct GSHL_Conversion {
        GSHL_TemplateKind kind;
        const char strings[3][32]; // 3 allowed aliases with 31 characters max
    } conversion[] = {
#define KIND(ENUM, NAME, TYPE, VA_ARG_TYPE, BIT_SHIFT, OPTS, ...)              \
    {GSHL_TEMPLATE_##ENUM, {__VA_ARGS__}},
        GSHL_TEMPLATE_KINDS
#undef KIND
    };

    GSHL_FORMAT_DEBUG_FUNC;

    GSHL_ASSERT(format != NULL);
    GSHL_ASSERT((*format)[0] == '{');

    *format += 1;

    const char *formatStart = *format;
    char *formatEnd = *format;
    for (; *formatEnd != '\0' && *formatEnd != '}' && *formatEnd != ';';
         formatEnd += 1) {
    }

    // TODO: Make this algorithm more efficient, ideas:
    // - state machine
    // - bitfield mask
    GSHL_ARRAY_FOREACH(conversion, struct GSHL_Conversion convert)
    {
        GSHL_ARRAY_FOREACH(convert.strings, char *string)
        {
            if (string == NULL || string[0] == '\0') {
                break;
            }

            if (memcmp(string, *format, formatEnd - formatStart) != 0) {
                continue;
            }

            *format = formatEnd;
            return convert.kind;
        }
    }

    GSHL_UNREACHABLE("Unknown format: %.*s", (int)(formatEnd - formatStart),
                     formatStart);

    return GSHL_TEMPLATE_NONE;
}

void GSHL_free_FormatWrapperResult(struct GSHL_FormatWrapperResult *result)
{
    free(result->string);

    if (result->templates.items != NULL) {
        free(result->templates.items);
    }

    *result = (struct GSHL_FormatWrapperResult){0};
}

GSHL_FormatWrapperResult
GSHL_format_wrapper(const char *restrict format,
                    const GSHL_FormatWrapperOpts formatOpts, ...)
{
    va_list args;
    va_start(args, formatOpts);

    const GSHL_FormatWrapperResult result =
        GSHL_format_wrapperv(format, formatOpts, args);

    va_end(args);

    return result;
}

void *GSHL_format_memwrite(void **bufP, const void *src, const usize len)
{
    void *dest = *bufP;

    memcpy(bufP, src, len);
    bufP += len;

    return dest;
}

usize GSHL_format_wrapper_result_countv(const char *restrict format,
                                        const GSHL_FormatWrapperOpts opts,
                                        va_list args)
{
    GSHL_UNUSED(format);
    GSHL_UNUSED(opts);
    GSHL_UNUSED(args);

    return 0;
}

usize GSHL_format_count_templates(char **formatP)
{
    GSHL_ASSERT(formatP != NULL);

    usize template_count = 0;
    char *formatEnd = NULL;
    isize parenBalance = 0;

    for (formatEnd = *formatP; *formatEnd != '\0'; formatEnd += 1) {
        const char *formatC = formatEnd;
        /*
         * Backward compatible format specifiers
         */
        if (formatC[0] == '%') {
            switch (formatC[1]) {
#define T(SPECIFIER, ENUM, NAME, TYPE, VA_ARG_TYPE, OPTS) case SPECIFIER:
#define TV(SPECIFIER, ENUM, NAME, TYPE, VALUE, OPTS) case SPECIFIER:
#define TL(...)
            case 'o':
                GSHL_FILL_TEMPLATE
                template_count += 1;
                formatEnd = (char *)formatC + 1;
                break;
#undef T
#undef TL
#undef TV
            case 'l':
                switch (formatC[2]) {
#define T(...)
#define TV(...)
#define TL(SPECIFIER, ENUM, NAME, TYPE, VA_ARG_TYPE, OPTS) case SPECIFIER:
                    GSHL_FILL_TEMPLATE
                    template_count += 1;
                    formatEnd = (char *)formatC + 1;
                    break;
#undef T
#undef TL
#undef TV
                default:
                    GSHL_UNREACHABLE(
                        "Unknown template specifier \"%.*s\", format: \"%s\"",
                        3, formatC, *formatP);
                }
                break;
            default:
                GSHL_UNREACHABLE(
                    "Unknown template specifier \"%.*s\", format: \"%s\"", 2,
                    formatC, *formatP);
            }
        }
        else if ((formatC[0] == '}' && formatC[1] == '}') ||
                 (formatC[0] == '{' && formatC[1] == '{')) {

            formatEnd = (char *)formatC + 1;
            template_count += 1;
        }
        else if (formatC[0] == '{') {

            parenBalance += 1;

            for (; formatC[0] != '\0' && formatC[1] != '}'; formatC += 1) {
            }

            assert(formatC[0] != '\0');
            assert(formatC[1] == '}');

            parenBalance -= 1;

            formatEnd = (char *)formatC + 1;
            template_count += 1;
        }
    }

    GSHL_ASSERT(parenBalance == 0);
    *formatP = formatEnd;

    return template_count;
}

GSHL_FormatWrapperResult
GSHL_format_wrapperv(const char *restrict format,
                     const GSHL_FormatWrapperOpts formatOpts, va_list args)
{
    GSHL_FormatWrapperResult result = {0};

    char *formatEnd = (char *)format;
    result.templates.count = GSHL_format_count_templates(&formatEnd);

    GSHL_FORMAT_DEBUG_PRINT("formatEnd = (%p)\n", formatEnd);
    GSHL_FORMAT_DEBUG_PRINT("formatEnd - formatStart = %li\n",
                            formatEnd - format);
    GSHL_FORMAT_DEBUG_PRINT("len(format) = %lu\n", strlen(format));

    GSHL_ASSERT(formatEnd - format == (isize)strlen(format));

    // result.count = GSHL_format_wrapper_result_countv(format, opts, args);

    /*
     * Fill templates
     */
    if (result.templates.count == 0) {
        result.count = formatEnd - format;
    }
    else {
        result.templates.items =
            calloc(result.templates.count, sizeof(*result.templates.items));
        assert(result.templates.items != NULL);

        usize template_index = 0;

        for (char *formatC = (char *)format; *formatC != '\0'; formatC += 1) {
            GSHL_Template *t = &result.templates.items[template_index];

            /*
             * Backward compatible format specifiers
             */
            if (formatC[0] == '%') {
                t->formatStart = formatC;
                t->formatEnd = formatC + 2;

                // %%
                t->character = "\0%"[formatC[1] == '%'];

                switch (formatC[1]) {
#define T(SPECIFIER, ENUM, NAME, TYPE, VA_ARG_TYPE, OPTS)                      \
    case SPECIFIER: {                                                          \
        t->kind = GSHL_TEMPLATE_##ENUM;                                        \
        t->NAME = va_arg(args, VA_ARG_TYPE);                                   \
        t->opts.NAME = (struct GSHL_TemplateOpts_##NAME)OPTS;                  \
        t->write.NAME = write_##NAME;                                          \
        t->write.NAME(NULL, t);                                                \
    } break;
#define TV(SPECIFIER, ENUM, NAME, TYPE, VALUE, OPTS)                           \
    case SPECIFIER: {                                                          \
        t->kind = GSHL_TEMPLATE_##ENUM;                                        \
        t->NAME = VALUE;                                                       \
        t->opts.NAME = (struct GSHL_TemplateOpts_##NAME)OPTS;                  \
        t->write.NAME = write_##NAME;                                          \
        t->write.NAME(NULL, t);                                                \
    } break;
#define TL(...)
                    GSHL_FILL_TEMPLATE
#undef T
#undef TL
#undef TV

                case 'l': {
                    t->formatEnd = formatC + 3;

                    switch (formatC[2]) {
#define T(...)
#define TV(...)
#define TL(SPECIFIER, ENUM, NAME, TYPE, VA_ARG_TYPE, OPTS)                     \
    case SPECIFIER: {                                                          \
        t->kind = GSHL_TEMPLATE_##ENUM;                                        \
        t->NAME = va_arg(args, VA_ARG_TYPE);                                   \
        t->opts.NAME = (struct GSHL_TemplateOpts_##NAME)OPTS;                  \
        t->write.NAME = write_##NAME;                                          \
        t->write.NAME(NULL, t);                                                \
    } break;
                        GSHL_FILL_TEMPLATE
#undef T
#undef TL
#undef TV
                    }
                } break;

                default:
                    GSHL_UNREACHABLE(
                        "Template not implemented yet '%c', format: \"%s\"",
                        formatC[1], format);
                }

                result.count += t->count;

                formatC = t->formatEnd - 1;
                template_index += 1;
            }
            else if ((formatC[0] == '}' && formatC[1] == '}') ||
                     (formatC[0] == '{' && formatC[1] == '{')) {

                t->formatStart = formatC;
                t->formatEnd = formatC + 2;
                t->kind = GSHL_TEMPLATE_CHAR;
                t->character = formatC[1];
                t->write.character = write_character;
                t->write.character(NULL, t);

                result.count += t->count;

                formatC = t->formatEnd - 1;
                template_index += 1;
            }
            else if (formatC[0] == '{') {
                t->formatStart = formatC;

                t->kind = GSHL_template_kind_from_str(&formatC);
                switch (t->kind) {
#define KIND(ENUM, NAME, TYPE, VA_ARG_TYPE, BIT_SHIFT, OPTS, ...)              \
    case GSHL_TEMPLATE_##ENUM:                                                 \
        t->write.NAME = write_##NAME;                                          \
        t->NAME = va_arg(args, VA_ARG_TYPE);                                   \
        t->write.NAME(NULL, t);                                                \
        break;
                    GSHL_TEMPLATE_KINDS
#undef KIND
                default:
                    GSHL_UNREACHABLE("Unknown kind: %d", t->kind);
                    break;
                }

                for (; formatC[0] != '}'; formatC += 1) {
                }

                assert(formatC[0] == '}');
                t->formatEnd = formatC + 1;

                result.count += t->count;
                formatC = t->formatEnd - 1;
                template_index += 1;
            }
            else {
                result.count += 1;
            }
        }
    }

#if GSHL_FORMAT_DEBUG & 0
    GSHL_ARRAYN_FOREACH(result.templates.items, result.templates.count,
                        GSHL_Template template)
    {
        fprintf(stderr,
                "{\n"
                "  format      = \"%s\"\n"
                "  formatStart = (%p) \"%s\"\n"
                "  formatEnd   = (%p) \"%s\"\n"
                "  isize       = %li\n"
                "  usize       = %lu\n"
                "  void *      = %p\n"
                "  count       = %lu\n"
                "  kind        = %u\n"
                "}\n",
                format, template.formatStart, template.formatStart,
                template.formatEnd, template.formatEnd, template.isize,
                template.usize, template.pointer, template.count,
                template.kind);
    }
#endif

    const usize prefixLen =
        (formatOpts.prefix != NULL) ? strlen(formatOpts.prefix) : 0;
    const usize suffixLen =
        (formatOpts.suffix != NULL) ? strlen(formatOpts.suffix) : 0;

    result.count += prefixLen + suffixLen;

#if GSHL_FORMAT_DEBUG > 0
    GSHL_FORMAT_DEBUG_PRINT("result.count = %lu\n", result.count);
    result.string = malloc((result.count + 1) * sizeof(*result.string));
    memset(result.string, ' ', result.count * sizeof(*result.string));
    result.string[result.count] = '\0';
#else
    result.string = calloc(result.count + 1, sizeof(*result.string));
#endif
    GSHL_ASSERT(result.string != NULL);

    {
        char *formatStart = (char *)format;
        char *output = result.string;

        memcpy(output, formatOpts.prefix, prefixLen);
        output += prefixLen;

        GSHL_FORMAT_DEBUG_PRINT(" result.string = \"%s\"\n", result.string);
        GSHL_ARRAYN_FOREACH_MUT(result.templates.items, result.templates.count,
                                const GSHL_Template *const template)
        {
            GSHL_ASSERT(formatStart <= template->formatStart);
            GSHL_ASSERT(formatStart <= template->formatEnd &&
                        template->formatEnd <= formatEnd);

            const usize len = template->formatStart - formatStart;
            memcpy(output, formatStart, len);
            output += len;

            GSHL_FORMAT_DEBUG_PRINT(
                len > 0 ? " result.string = \"%s\"\n" : "\r", result.string);

#if GSHL_FORMAT_DEBUG
            {
                fprintf(stderr,
                        "{\n"
                        "  format      = \"%s\"\n"
                        "  formatStart = (%p) \"%s\"\n"
                        "  formatEnd   = (%p) \"%s\"\n"
                        "  isize       = %li\n"
                        "  usize       = %lu\n"
                        "  void *      = %p\n"
                        "  count       = %lu\n"
                        "  kind        = %u\n"
                        "}\n",
                        format, template.formatStart, template.formatStart,
                        template.formatEnd, template.formatEnd, template.isize,
                        template.usize, template.pointer, template.count,
                        template.kind);
            }
#endif
            switch (template->kind) {
#define KIND(ENUM, NAME, TYPE, VA_ARG_TYPE, BIT_SHIFT, OPTS, ...)              \
    case GSHL_TEMPLATE_##ENUM:                                                 \
        output += template->write.NAME(output, (GSHL_Template *)template);     \
        GSHL_FORMAT_DEBUG_PRINT(" result.string = \"%s\"\n", result.string);   \
        break;
                GSHL_TEMPLATE_KINDS
#undef KIND
            default:
                GSHL_UNREACHABLE(
                    "Template write not implemented yet '%.*s', format: "
                    "\"%s\"",
                    (int)(template->formatEnd - template->formatStart),
                    template->formatStart, format);
                break;
            }

            formatStart = template->formatEnd;
        }

        memcpy(output, formatStart, formatEnd - formatStart);
        output += formatEnd - formatStart;

        memcpy(output, formatOpts.suffix, suffixLen);
        output += suffixLen;
    }

    return result;
}

#ifdef GSHL_TESTS


GSHL_TEST(format_wrapper)
{
#    define TEST_FORMAT_WRAPPER_RESULT(EXPECTED_STRING, EXPECTED_COUNT,        \
                                       ADDITIONAL_TESTS, ...)                  \
        {                                                                      \
            GSHL_FormatWrapperResult result =                                  \
                GSHL_format_wrapper(__VA_ARGS__);                              \
            GSHL_TEST_STR_EQUAL(result.string, EXPECTED_STRING);               \
            GSHL_TEST_EQUAL(result.count, EXPECTED_COUNT);                     \
            do {                                                               \
                GSHL_Template *templates = result.templates.items;             \
                GSHL_UNUSED(templates);                                        \
                ADDITIONAL_TESTS                                               \
            } while (0);                                                       \
            GSHL_free_FormatWrapperResult(&result);                            \
        }

    const GSHL_FormatWrapperOpts opts = {};

    TEST_FORMAT_WRAPPER_RESULT("", 0, {}, "", opts);
    TEST_FORMAT_WRAPPER_RESULT("Hello world!\n", 13, {}, "Hello world!\n",
                               opts);
    TEST_FORMAT_WRAPPER_RESULT(
        "3245", 4,
        {
            GSHL_TEST_EQUAL(result.templates.count, 1);
            GSHL_TEST_EQUAL(templates[0].kind, GSHL_TEMPLATE_I32);
            GSHL_TEST_EQUAL(templates[0].i32, 3245);
            GSHL_TEST_EQUAL(templates[0].count, 4);
        },
        "%i", opts, 3245);
    TEST_FORMAT_WRAPPER_RESULT(
        "1 -2 3", 6,
        {
            GSHL_TEST_EQUAL(result.templates.count, 3);
            GSHL_TEST_EQUAL(templates[0].kind, GSHL_TEMPLATE_I32);
            GSHL_TEST_EQUAL(templates[0].i32, 1);
            GSHL_TEST_EQUAL(templates[0].count, 1);
            GSHL_TEST_EQUAL(templates[1].kind, GSHL_TEMPLATE_I32);
            GSHL_TEST_EQUAL(templates[1].i32, -2);
            GSHL_TEST_EQUAL(templates[1].count, 2);
            GSHL_TEST_EQUAL(templates[2].kind, GSHL_TEMPLATE_I32);
            GSHL_TEST_EQUAL(templates[2].i32, 3);
            GSHL_TEST_EQUAL(templates[2].count, 1);
        },
        "%i %i %i", opts, 1, -2, 3);

    TEST_FORMAT_WRAPPER_RESULT(
        "    -485   ", 11,
        {
            GSHL_TEST_EQUAL(result.templates.count, 1);
            GSHL_TEST_EQUAL(templates[0].kind, GSHL_TEMPLATE_I32);
            GSHL_TEST_EQUAL(templates[0].i32, -485);
            GSHL_TEST_EQUAL(templates[0].count, 4);
        },
        "    %i   ", opts, -485);

    TEST_FORMAT_WRAPPER_RESULT(
        "Hello world!", 12,
        {
            GSHL_TEST_EQUAL(result.templates.count, 1);
            GSHL_TEST_EQUAL(templates[0].kind, GSHL_TEMPLATE_CSTRING);
            GSHL_TEST_STR_EQUAL(templates[0].cstring, "Hello world!");
            GSHL_TEST_EQUAL(templates[0].count, 12);
        },
        "%s", opts, "Hello world!");

    TEST_FORMAT_WRAPPER_RESULT(
        "  Foo  bar  baz ", 16,
        {
            GSHL_TEST_EQUAL(result.templates.count, 3);
            GSHL_TEST_EQUAL(templates[0].kind, GSHL_TEMPLATE_CSTRING);
            GSHL_TEST_STR_EQUAL(templates[0].cstring, "Foo");
            GSHL_TEST_EQUAL(templates[0].count, 3);
            GSHL_TEST_EQUAL(templates[1].kind, GSHL_TEMPLATE_CSTRING);
            GSHL_TEST_STR_EQUAL(templates[1].cstring, "bar");
            GSHL_TEST_EQUAL(templates[1].count, 3);
            GSHL_TEST_EQUAL(templates[2].kind, GSHL_TEMPLATE_CSTRING);
            GSHL_TEST_STR_EQUAL(templates[2].cstring, "baz");
            GSHL_TEST_EQUAL(templates[2].count, 3);
        },
        "  %s  %s  %s ", opts, "Foo", "bar", "baz");

    TEST_FORMAT_WRAPPER_RESULT(
        "count = 5", 9,
        {
            GSHL_TEST_EQUAL(result.templates.count, 2);
            GSHL_TEST_EQUAL(templates[0].kind, GSHL_TEMPLATE_CSTRING);
            GSHL_TEST_STR_EQUAL(templates[0].cstring, "count");
            GSHL_TEST_EQUAL(templates[0].count, 5);
            GSHL_TEST_EQUAL(templates[1].kind, GSHL_TEMPLATE_I32);
            GSHL_TEST_EQUAL(templates[1].i32, 5);
            GSHL_TEST_EQUAL(templates[1].count, 1);
        },
        "%s = %i", opts, "count", 5);

    TEST_FORMAT_WRAPPER_RESULT(
        "i32: 42", 7,
        {
            GSHL_TEST_EQUAL(result.templates.count, 1);
            GSHL_TEST_EQUAL(templates[0].kind, GSHL_TEMPLATE_I32);
            GSHL_TEST_EQUAL(templates[0].i32, 42);
            GSHL_TEST_EQUAL(templates[0].count, 2);
        },
        "i32: %i", opts, 42)

    TEST_FORMAT_WRAPPER_RESULT(
        "u32: 42", 7,
        {
            GSHL_TEST_EQUAL(result.templates.count, 1);
            GSHL_TEST_EQUAL(templates[0].kind, GSHL_TEMPLATE_U32);
            GSHL_TEST_EQUAL(templates[0].u32, 42);
            GSHL_TEST_EQUAL(templates[0].count, 2);
        },
        "u32: %u", opts, 42)

    TEST_FORMAT_WRAPPER_RESULT(
        "char: F", 7,
        {
            GSHL_TEST_EQUAL(result.templates.count, 1);
            GSHL_TEST_EQUAL(templates[0].kind, GSHL_TEMPLATE_CHAR);
            GSHL_TEST_EQUAL(templates[0].character, 'F');
            GSHL_TEST_EQUAL(templates[0].count, 1);
        },
        "char: %c", opts, 'F')

    TEST_FORMAT_WRAPPER_RESULT(
        "void*: nil", 10,
        {
            GSHL_TEST_EQUAL(result.templates.count, 1);
            GSHL_TEST_EQUAL(templates[0].kind, GSHL_TEMPLATE_POINTER);
            GSHL_TEST_EQUAL(templates[0].pointer, NULL);
            GSHL_TEST_EQUAL(templates[0].count, 3);
        },
        "void*: %p", opts, NULL)

    TEST_FORMAT_WRAPPER_RESULT(
        "void*: 0x7ffc0d1b64f4", 21,
        {
            GSHL_TEST_EQUAL(result.templates.count, 1);
            GSHL_TEST_EQUAL(templates[0].kind, GSHL_TEMPLATE_POINTER);
            GSHL_TEST_EQUAL(templates[0].pointer, (void *)0x7ffc0d1b64f4);
            GSHL_TEST_EQUAL(templates[0].count, 14);
        },
        "void*: %p", opts, (void *)0x7ffc0d1b64f4)

    TEST_FORMAT_WRAPPER_RESULT(
        "%", 1,
        {
            GSHL_TEST_EQUAL(result.templates.count, 1);
            GSHL_TEST_EQUAL(templates[0].kind, GSHL_TEMPLATE_CHAR);
            GSHL_TEST_EQUAL(templates[0].character, '%');
            GSHL_TEST_EQUAL(templates[0].count, 1);
        },
        "%%", opts)

    TEST_FORMAT_WRAPPER_RESULT(
        "69", 2,
        {
            GSHL_TEST_EQUAL(result.templates.count, 1);
            GSHL_TEST_EQUAL(templates[0].kind, GSHL_TEMPLATE_HEXADECIMAL);
            GSHL_TEST_EQUAL(templates[0].hex, 0x69);
            GSHL_TEST_EQUAL(templates[0].count, 2);
        },
        "%x", opts, 105UL)

    TEST_FORMAT_WRAPPER_RESULT(
        "69 24", 5,
        {
            GSHL_TEST_EQUAL(result.templates.count, 2);
            GSHL_TEST_EQUAL(templates[0].kind, GSHL_TEMPLATE_HEXADECIMAL);
            GSHL_TEST_EQUAL(templates[0].hex, 105UL);
            GSHL_TEST_EQUAL(templates[0].count, 2);
            GSHL_TEST_EQUAL(templates[1].kind, GSHL_TEMPLATE_I32);
            GSHL_TEST_EQUAL(templates[1].i32, 24);
            GSHL_TEST_EQUAL(templates[1].count, 2);
        },
        "%x %i", opts, 105, 24)

    TEST_FORMAT_WRAPPER_RESULT(
        "{  }", 4,
        {
            GSHL_TEST_EQUAL(result.templates.count, 2);
            GSHL_TEST_EQUAL(templates[0].kind, GSHL_TEMPLATE_CHAR);
            GSHL_TEST_EQUAL(templates[0].character, '{');
            GSHL_TEST_EQUAL(templates[0].count, 1);
            GSHL_TEST_EQUAL(templates[1].kind, GSHL_TEMPLATE_CHAR);
            GSHL_TEST_EQUAL(templates[1].character, '}');
            GSHL_TEST_EQUAL(templates[1].count, 1);
        },
        "{{  }}", opts)

    TEST_FORMAT_WRAPPER_RESULT(
        "127 32767 -38453 235638 2349", 28,
        {
            GSHL_TEST_EQUAL(result.templates.count, 5);
            GSHL_TEST_EQUAL(templates[0].kind, GSHL_TEMPLATE_I8);
            GSHL_TEST_EQUAL(templates[0].i8, 127);
            GSHL_TEST_EQUAL(templates[0].count, 3);
            GSHL_TEST_EQUAL(templates[1].kind, GSHL_TEMPLATE_I16);
            GSHL_TEST_EQUAL(templates[1].i16, 32767);
            GSHL_TEST_EQUAL(templates[1].count, 5);
            GSHL_TEST_EQUAL(templates[2].kind, GSHL_TEMPLATE_I32);
            GSHL_TEST_EQUAL(templates[2].i32, -38453);
            GSHL_TEST_EQUAL(templates[2].count, 6);
            GSHL_TEST_EQUAL(templates[3].kind, GSHL_TEMPLATE_I64);
            GSHL_TEST_EQUAL(templates[3].i64, 235638);
            GSHL_TEST_EQUAL(templates[3].count, 6);
            GSHL_TEST_EQUAL(templates[4].kind, GSHL_TEMPLATE_ISIZE);
            GSHL_TEST_EQUAL(templates[4].isize, 2349);
            GSHL_TEST_EQUAL(templates[4].count, 4);
        },
        "{i8} {i16} {i32} {i64} {isize}", opts, 127, 32767, -38453, 235638,
        2349)

    TEST_FORMAT_WRAPPER_RESULT(
        "255, 3422, 12893, 393294, 34455677", 34,
        {
            GSHL_TEST_EQUAL(result.templates.count, 5);
            GSHL_TEST_EQUAL(templates[0].kind, GSHL_TEMPLATE_U8);
            GSHL_TEST_EQUAL(templates[0].u8, 255);
            GSHL_TEST_EQUAL(templates[0].count, 3);
            GSHL_TEST_EQUAL(templates[1].kind, GSHL_TEMPLATE_U16);
            GSHL_TEST_EQUAL(templates[1].u16, 3422);
            GSHL_TEST_EQUAL(templates[1].count, 4);
            GSHL_TEST_EQUAL(templates[2].kind, GSHL_TEMPLATE_U32);
            GSHL_TEST_EQUAL(templates[2].u32, 12893);
            GSHL_TEST_EQUAL(templates[2].count, 5);
            GSHL_TEST_EQUAL(templates[3].kind, GSHL_TEMPLATE_U64);
            GSHL_TEST_EQUAL(templates[3].u64, 393294);
            GSHL_TEST_EQUAL(templates[3].count, 6);
            GSHL_TEST_EQUAL(templates[4].kind, GSHL_TEMPLATE_USIZE);
            GSHL_TEST_EQUAL(templates[4].usize, 34455677);
            GSHL_TEST_EQUAL(templates[4].count, 8);
        },
        "{u8}, {u16}, {u32}, {u64}, {usize}", opts, 0xFF, 3422, 12893, 393294,
        34455677);

    // TEST_FORMAT_WRAPPER_RESULT(
    //     "3.14 3.14 3.14 3.14", 19,
    //     {
    //         GSHL_TEST_EQUAL(result.templates.count, 2);
    //         GSHL_TEST_EQUAL(templates[0].kind, GSHL_TEMPLATE_F32);
    //         GSHL_TEST_EQUAL(templates[0].f32, 3.14f);
    //         GSHL_TEST_EQUAL(templates[0].count, 4);
    //         GSHL_TEST_EQUAL(templates[1].kind, GSHL_TEMPLATE_F32);
    //         GSHL_TEST_EQUAL(templates[1].f32, 3.14f);
    //         GSHL_TEST_EQUAL(templates[1].count, 4);
    //         GSHL_TEST_EQUAL(templates[2].kind, GSHL_TEMPLATE_F64);
    //         GSHL_TEST_EQUAL(templates[2].f64, 3.14);
    //         GSHL_TEST_EQUAL(templates[2].count, 4);
    //         GSHL_TEST_EQUAL(templates[3].kind, GSHL_TEMPLATE_F64);
    //         GSHL_TEST_EQUAL(templates[3].f64, 3.14);
    //         GSHL_TEST_EQUAL(templates[3].count, 4);
    //     },
    //     "{f32} %f {f64} %lf", 3.14f, 3.14f, 3.14, 3.14);

#    if 1

#        define ALL_FORMAT                                                     \
            /* X(FORMAT, EXPECTED_FORMAT, VALUE, EXPECTED_COUNT) */            \
            X1("(%%b:  0b%b) ", "(%b:  0b10101) ", 0b10101, 5)                 \
            X1("(%%B:  %B) ", "(%B:  true) ", 1, 4)                            \
            X1("(%%c:  %c) ", "(%c:  F) ", 'F', 1)                             \
            X1("(%%d:  %d) ", "(%d:  -34) ", -34, 3)                           \
            X1("(%%i:  %i) ", "(%i:  -35) ", -35, 3)                           \
            X0("(%%f:  %f) ", "(%f:  -69.0) ", -69.f, 5)                       \
            X1("(%%p:  %p) ", "(%p:  0x7ffc0d1b64f4) ", 0x7ffc0d1b64f4, 14)    \
            X1("(%%p:  %p) ", "(%p:  nil) ", NULL, 3)                          \
            X1("(%%s:  \"%s\") ", "(%s:  \"Hello world!\") ", "Hello world!",  \
               12)                                                             \
            X1("(%%u:  %u) ", "(%u:  69) ", 69, 2)                             \
            X1("(%%x:  0x%x) ", "(%x:  0xff00ff) ", 0xFF00FF, 6)               \
            X1("(%%X:  0x%X) ", "(%X:  0xCAFEBABE) ", 0xCAFEBABE, 8)           \
            X0("(%%lb: 0b%lb) ", "(%lb: 0b1011) ", 0b1011, 4)                  \
            X0("(%%lf: %lf) ", "(%lf: 34.14) ", 34.14, 5)                      \
            X0("(%%li: %li) ", "(%li: -69) ", -69, 3)                          \
            X0("(%%lu: %lu) ", "(%lu: 69) ", 69, 2)                            \
            X0("(%%lx: 0x%lx) ", "(%lx: 0x123456789abcdef) ",                  \
               0x0123456789abcde, 15)                                          \
            X0("(%%lX: 0x%lX) ", "(%lX: 0X123456789ABCDEF) ",                  \
               0X0123456789ABCDEF, 15)                                         \
            X0("({{i8}}:    {i8}) ", "({i8}:    127) ", 127, 3)                \
            X0("({{i16}}:   {i16}) ", "({i16}:   -34) ", -34, 3)               \
            X0("({{i32}}:   {i32}) ", "({i32}:   -35) ", -35, 3)               \
            X0("({{i64}}:   {i64}) ", "({i64}:   -69) ", -69, 3)               \
            X0("({{isize}}: {isize}) ", "({isize}: -420) ", -420, 4)           \
            X0("({{u8}}:    {u8}) ", "({u8}:    255) ", 255, 3)                \
            X0("({{u16}}:   {u16}) ", "({u16}:   34) ", 34, 2)                 \
            X0("({{u32}}:   {u32}) ", "({u32}:   35) ", 35, 2)                 \
            X0("({{u64}}:   {u64}) ", "({u64}:   69) ", 69, 2)                 \
            X0("({{usize}}: {usize}) ", "({usize}: 420) ", 420, 3)             \
            X0("({{boolean}}: {boolean}) ", "({boolean}: false) ", false, 5)   \
            X0("({{boolean}}: {boolean}) ", "({boolean}: true) ", true, 4)     \
            X0("({{binary}}: 0b{binary}) ", "({binary}: 0b1011) ", 0b1011, 4)  \
            X0("({{f32}}: {f32}) ", "({f32}: {f32}) ", 0, 0)                   \
            X0("({{f64}}: {f64}) ", "({f64}: {f64}) ", 0, 0)

    {
        const char format[] = ""
#        define X0(...)
#        define X1(FORMAT, EXPECTED_FORMAT, VALUE, EXPECTED_COUNT) FORMAT
#        define X2(FORMAT, EXPECTED_FORMAT, VALUE, EXPECTED_COUNT) FORMAT
            ALL_FORMAT
#        undef X0
#        undef X1
#        undef X2
            ;

        const char expected[] = ""
#        define X0(...)
#        define X1(FORMAT, EXPECTED_FORMAT, VALUE, EXPECTED_COUNT)             \
            EXPECTED_FORMAT
            ALL_FORMAT
#        undef X0
#        undef X1
            ;

        const usize count = sizeof(expected) - 1;

        const usize template_count = 0
#        define X0(...)
#        define X1(FORMAT, EXPECTED_FORMAT, VALUE, EXPECTED_COUNT) +2
#        define X2(FORMAT, EXPECTED_FORMAT, VALUE, EXPECTED_COUNT) +3
            ALL_FORMAT
#        undef X0
#        undef X1
#        undef X2
            ;

        GSHL_FormatWrapperResult result =
            GSHL_format_wrapper(format, opts
#        define X0(...)
#        define X1(FORMAT, EXPECTED_FORMAT, VALUE, EXPECTED_COUNT) , VALUE
                                            ALL_FORMAT
#        undef X0
#        undef X1
            );

        GSHL_TEST_STR_EQUAL(result.string, expected, .align_on_new_line = true);
        GSHL_TEST_EQUAL(result.count, count);

        usize template_index = 0;
        do {
            GSHL_Template *templates = result.templates.items;
            GSHL_UNUSED(templates);

            GSHL_TEST_EQUAL(result.templates.count, template_count);

#        define X0(...)
#        define X1(FORMAT, EXPECTED_FORMAT, VALUE, EXPECTED_COUNT)             \
            /* printf("%s\n", FORMAT); */                                      \
            GSHL_TEST_EQUAL(templates[template_index].kind,                    \
                            GSHL_TEMPLATE_CHAR);                               \
            GSHL_TEST_EQUAL(templates[template_index].count, 1);               \
            GSHL_TEST_EQUAL(templates[template_index].character, '%');         \
            GSHL_TEST_EQUAL(templates[template_index + 1].count,               \
                            EXPECTED_COUNT);                                   \
            template_index += 2;
#        define X2(...)
            ALL_FORMAT
#        undef X0
#        undef X1
#        undef X2
        } while (0);

        GSHL_free_FormatWrapperResult(&result);
    }
#    endif

#    undef TEST_FORMAT_WRAPPER_RESULT
}

#    undef GSHL_FORMAT_DEBUG_FUNC
#    undef GSHL_FORMAT_DEBUG_PRINT

#endif // GSHL_TEST
#ifdef GSHL_SOURCE_CODE_MAPPING
#    line 2 "src/format/usize.c"
#endif // GSHL_SOURCE_CODE_MAPPING


#include <assert.h>
#include <stdlib.h>
#include <string.h>

usize write_usize(char *buf, GSHL_Template *t_mut)
{
    const GSHL_Template *const t = t_mut;

    GSHL_ASSERT(t->kind &
                (GSHL_TEMPLATE_U8 | GSHL_TEMPLATE_U16 | GSHL_TEMPLATE_U32 |
                 GSHL_TEMPLATE_U64 | GSHL_TEMPLATE_USIZE));

    usize remaining = t->u8 * !!(t->kind & GSHL_TEMPLATE_U8) +
                      t->u16 * !!(t->kind & GSHL_TEMPLATE_U16) +
                      t->u32 * !!(t->kind & GSHL_TEMPLATE_U32) +
                      t->u64 * !!(t->kind & GSHL_TEMPLATE_U64) +
                      t->usize * !!(t->kind & GSHL_TEMPLATE_USIZE);

    if (buf == NULL) {
        t_mut->count = 0;

        do {
            remaining /= 10;
            t_mut->count += 1;
        } while (remaining != 0);

        return t->count;
    }

    if (remaining == 0) {
        buf[0] = '0';
        return t->count;
    }

    for (isize i = t->count - 1; i >= 0 && remaining > 0;
         --i, remaining /= 10) {
        buf[i] = '0' + (remaining % 10);
    }

    GSHL_ASSERT(remaining == 0);

    return t->count;
}

#ifdef GSHL_TESTS


GSHL_TEST(write_usize)
{
#    define TEST_WRITE_USIZE(NUMBER, EXPECTED_STRING, ...)                     \
        {                                                                      \
            char buffer[256] = {0};                                            \
            struct GSHL_Template template = {                                  \
                .kind = GSHL_TEMPLATE_USIZE,                                   \
                .usize = NUMBER,                                               \
                .opts.usize = {__VA_ARGS__},                                   \
            };                                                                 \
            const usize count = GSHL_STACK_STRING_LEN(EXPECTED_STRING);        \
            GSHL_TEST_EQUAL(write_usize(NULL, &template), count);              \
            GSHL_TEST_EQUAL(write_usize(buffer, &template), count);            \
            GSHL_TEST_STR_EQUAL(buffer, EXPECTED_STRING);                      \
        }

    {
        char buffer[256] = {0};
        struct GSHL_Template template = {
            .kind = GSHL_TEMPLATE_USIZE,
            .usize = 0,
            .opts.usize = {},
        };
        const usize count = GSHL_STACK_STRING_LEN("0");
        GSHL_TEST_EQUAL(write_usize(NULL, &template), count);
        GSHL_TEST_EQUAL(write_usize(buffer, &template), count);
        GSHL_TEST_STR_EQUAL(buffer, "0");
    }
    TEST_WRITE_USIZE(0, "0");
    TEST_WRITE_USIZE(3245, "3245");
    TEST_WRITE_USIZE(34948239, "34948239");
    // TEST_WRITE_usize(usize_MAX, 10, "4294967295");
    // TEST_WRITE_usize(usize_MIN, 1, "0");

#    undef TEST_WRITE_usize
}

#endif
#ifdef GSHL_SOURCE_CODE_MAPPING
#    line 3 "src/format/u8.c"
#endif // GSHL_SOURCE_CODE_MAPPING


#include <assert.h>

usize write_u8(char *buf, GSHL_Template *t_mut)
{
    const GSHL_Template *const t = t_mut;

    GSHL_ASSERT(t->kind == GSHL_TEMPLATE_U8);

    return write_usize(buf, t_mut);
}
#ifdef GSHL_SOURCE_CODE_MAPPING
#    line 3 "src/format/u64.c"
#endif // GSHL_SOURCE_CODE_MAPPING


#include <assert.h>

usize write_u64(char *buf, GSHL_Template *t_mut)
{
    const GSHL_Template *const t = t_mut;

    GSHL_ASSERT(t->kind == GSHL_TEMPLATE_U64);

    return write_usize(buf, t_mut);
}
#ifdef GSHL_SOURCE_CODE_MAPPING
#    line 3 "src/format/u32.c"
#endif // GSHL_SOURCE_CODE_MAPPING


#include <assert.h>

usize write_u32(char *buf, GSHL_Template *t_mut)
{
    const GSHL_Template *const t = t_mut;

    GSHL_ASSERT(t->kind == GSHL_TEMPLATE_U32);

    return write_usize(buf, t_mut);
}
#ifdef GSHL_SOURCE_CODE_MAPPING
#    line 3 "src/format/u16.c"
#endif // GSHL_SOURCE_CODE_MAPPING


#include <assert.h>

usize write_u16(char *buf, GSHL_Template *t_mut)
{
    const GSHL_Template *const t = t_mut;

    GSHL_ASSERT(t->kind == GSHL_TEMPLATE_U16);

    return write_usize(buf, t_mut);
}
#ifdef GSHL_SOURCE_CODE_MAPPING
#    line 2 "src/format/pointer.c"
#endif // GSHL_SOURCE_CODE_MAPPING


#include <assert.h>
#include <string.h>

usize write_pointer(char *buf, GSHL_Template *t_mut)
{
    const GSHL_Template *const t = t_mut;

    GSHL_ASSERT(t->kind == GSHL_TEMPLATE_POINTER);

    static const char prefix[] = "0x";
    static const usize prefixLen = GSHL_STACK_STRING_LEN(prefix);

    if (buf == NULL) {
        if (t->pointer == NULL) {
            return t_mut->count = GSHL_STACK_STRING_LEN("nil");
        }

        t_mut->count = prefixLen;

        uintptr_t address = (uintptr_t)t->pointer;
        do {
            address >>= 4;
            t_mut->count += 1;
        } while (address != 0);

        return t->count;
    }

    if (t->pointer == NULL) {
        GSHL_ASSERT(t->count == 3);
        memcpy(buf, "nil", t->count);
        return t->count;
    }

    uintptr_t address = (uintptr_t)t->pointer;

    static const char conversion[][17] = {
        [false] = "0123456789abcdef",
        [true] = "0123456789ABCDEF",
    };

    // Fill the buffer with hexadecimal representation
    for (isize i = t->count - prefixLen - 1; i >= 0 && address > 0;
         --i, address >>= 4) {
        buf[prefixLen + i] =
            conversion[t->opts.pointer.uppercase][address % 16];
    }

    memcpy(buf, prefix, prefixLen);

    return t->count;
}

#ifdef GSHL_TESTS


GSHL_TEST(write_pointer)
{
#    define TEST_WRITE_POINTER(NUMBER, EXPECTED_STRING, ...)                   \
        {                                                                      \
            char buffer[256] = {0};                                            \
            struct GSHL_Template template = {                                  \
                .kind = GSHL_TEMPLATE_POINTER,                                 \
                .pointer = NUMBER,                                             \
                .opts.pointer = {__VA_ARGS__},                                 \
            };                                                                 \
            const usize count = GSHL_STACK_STRING_LEN(EXPECTED_STRING);        \
            GSHL_TEST_EQUAL(write_pointer(NULL, &template), count);            \
            GSHL_TEST_EQUAL(write_pointer(buffer, &template), count);          \
            GSHL_TEST_STR_EQUAL(buffer, EXPECTED_STRING);                      \
        }

    TEST_WRITE_POINTER(0, "nil");
    TEST_WRITE_POINTER(NULL, "nil");
    TEST_WRITE_POINTER((void *)0x7ffc0d1b64f4, "0x7ffc0d1b64f4");

#    undef TEST_WRITE_POINTER
}

#endif
#ifdef GSHL_SOURCE_CODE_MAPPING
#    line 2 "src/format/mod.c"
#endif // GSHL_SOURCE_CODE_MAPPING

#include <stdarg.h>
#include <stdlib.h>

char *GSHL_format(const char *restrict format, ...)
{
    va_list args;

    va_start(args, format);
    char *string = GSHL_formatv(format, args);
    va_end(args);

    return string;
}

char *GSHL_formatv(const char *restrict format, va_list args)
{
    static const GSHL_FormatWrapperOpts opts = {};

    GSHL_FormatWrapperResult result = GSHL_format_wrapperv(format, opts, args);

    if (result.templates.items != NULL) {
        free(result.templates.items);
        result.templates.items = NULL;
        result.templates.count = 0;
    }

    return result.string;
}

char *GSHL_formatln(const char *restrict format, ...)
{
    static const GSHL_FormatWrapperOpts opts = {
        .suffix = "\n",
    };

    va_list args;
    va_start(args, format);

    GSHL_FormatWrapperResult result = GSHL_format_wrapperv(format, opts, args);
    va_end(args);

    if (result.templates.items != NULL) {
        free(result.templates.items);
        result.templates.items = NULL;
        result.templates.count = 0;
    }

    return result.string;
}
#ifdef GSHL_SOURCE_CODE_MAPPING
#    line 2 "src/format/isize.c"
#endif // GSHL_SOURCE_CODE_MAPPING


#include <assert.h>
#include <stdlib.h>
#include <string.h>

usize write_isize(char *buf, GSHL_Template *t_mut)
{
    const GSHL_Template *const t = t_mut;

    GSHL_ASSERT(t->kind &
                (GSHL_TEMPLATE_I8 | GSHL_TEMPLATE_I16 | GSHL_TEMPLATE_I32 |
                 GSHL_TEMPLATE_I64 | GSHL_TEMPLATE_ISIZE));

    isize remaining = t->i32 * !!(t->kind & GSHL_TEMPLATE_I8) +
                      t->i32 * !!(t->kind & GSHL_TEMPLATE_I16) +
                      t->i32 * !!(t->kind & GSHL_TEMPLATE_I32) +
                      t->i64 * !!(t->kind & GSHL_TEMPLATE_I64) +
                      t->isize * !!(t->kind & GSHL_TEMPLATE_ISIZE);

    if (buf == NULL) {
        if (remaining < 0) {
            t_mut->count = GSHL_STACK_STRING_LEN("-");
            remaining *= -1;
        }
        else {
            t_mut->count = 0;
        }

        do {
            remaining /= 10;
            t_mut->count += 1;
        } while (remaining != 0);

        return t->count;
    }

    if (remaining == 0) {
        GSHL_ASSERT(t->count == 1);
        buf[0] = '0';
        return t->count;
    }

    if (remaining < 0) {
        buf[0] = '-';
        remaining *= -1;
    }

    for (isize i = t->count - 1; i >= 0 && remaining > 0;
         --i, remaining /= 10) {
        buf[i] = '0' + (remaining % 10);
    }

    return t->count;
}

#ifdef GSHL_TESTS


GSHL_TEST(write_isize)
{
#    define TEST_WRITE_ISIZE(NUMBER, EXPECTED_STRING, ...)                     \
        {                                                                      \
            char buffer[256] = {0};                                            \
            struct GSHL_Template template = {                                  \
                .kind = GSHL_TEMPLATE_ISIZE,                                   \
                .isize = NUMBER,                                               \
                .opts.isize = {__VA_ARGS__},                                   \
            };                                                                 \
            const usize count = GSHL_STACK_STRING_LEN(EXPECTED_STRING);        \
            GSHL_TEST_EQUAL(write_isize(NULL, &template), count);              \
            GSHL_TEST_EQUAL(write_isize(buffer, &template), count);            \
            GSHL_TEST_STR_EQUAL(buffer, EXPECTED_STRING);                      \
        }

    TEST_WRITE_ISIZE(0, "0");
    TEST_WRITE_ISIZE(3245, "3245");
    TEST_WRITE_ISIZE(34948239, "34948239");
    TEST_WRITE_ISIZE(-0, "0");
    TEST_WRITE_ISIZE(-2, "-2");
    TEST_WRITE_ISIZE(-5, "-5");
    TEST_WRITE_ISIZE(-534, "-534");
    TEST_WRITE_ISIZE(-69, "-69");
    // TEST_WRITE_isize(ISIZE_MAX, 10, "2147483647");
#    undef TEST_WRITE_isize
}

#endif
#ifdef GSHL_SOURCE_CODE_MAPPING
#    line 3 "src/format/i8.c"
#endif // GSHL_SOURCE_CODE_MAPPING


#include <assert.h>

usize write_i8(char *buf, GSHL_Template *t_mut)
{
    const GSHL_Template *const t = t_mut;

    GSHL_ASSERT(t->kind == GSHL_TEMPLATE_I8);

    return write_isize(buf, t_mut);
}
#ifdef GSHL_SOURCE_CODE_MAPPING
#    line 3 "src/format/i64.c"
#endif // GSHL_SOURCE_CODE_MAPPING


#include <assert.h>

usize write_i64(char *buf, GSHL_Template *t_mut)
{
    const GSHL_Template *const t = t_mut;

    GSHL_ASSERT(t->kind == GSHL_TEMPLATE_I64);

    return write_isize(buf, t_mut);
}
#ifdef GSHL_SOURCE_CODE_MAPPING
#    line 3 "src/format/i32.c"
#endif // GSHL_SOURCE_CODE_MAPPING


#include <assert.h>

usize write_i32(char *buf, GSHL_Template *t_mut)
{
    const GSHL_Template *const t = t_mut;

    GSHL_ASSERT(t->kind == GSHL_TEMPLATE_I32);

    return write_isize(buf, t_mut);
}
#ifdef GSHL_SOURCE_CODE_MAPPING
#    line 3 "src/format/i16.c"
#endif // GSHL_SOURCE_CODE_MAPPING


#include <assert.h>

usize write_i16(char *buf, GSHL_Template *t_mut)
{
    const GSHL_Template *const t = t_mut;

    GSHL_ASSERT(t->kind == GSHL_TEMPLATE_I16);

    return write_isize(buf, t_mut);
}

#ifdef GSHL_TESTS


GSHL_TEST(write_i16)
{
#    define TEST_WRITE_I16(NUMBER, EXPECTED_STRING, ...)                       \
        {                                                                      \
            char buffer[256] = {0};                                            \
            struct GSHL_Template template = {                                  \
                .kind = GSHL_TEMPLATE_I16,                                     \
                .i16 = NUMBER,                                                 \
                .opts.i16 = {__VA_ARGS__},                                     \
            };                                                                 \
            const usize count = GSHL_STACK_STRING_LEN(EXPECTED_STRING);        \
            GSHL_TEST_EQUAL(write_i16(NULL, &template), count);                \
            GSHL_TEST_EQUAL(write_i16(buffer, &template), count);              \
            GSHL_TEST_STR_EQUAL(buffer, EXPECTED_STRING);                      \
        }

    TEST_WRITE_I16(32767, "32767");
#    undef TEST_WRITE_isize
}

#endif
#ifdef GSHL_SOURCE_CODE_MAPPING
#    line 4 "src/format/hex.c"
#endif // GSHL_SOURCE_CODE_MAPPING

#include <assert.h>
#include <string.h>

usize write_hex(char *buf, GSHL_Template *t_mut)
{
    const GSHL_Template *const t = t_mut;

    GSHL_ASSERT(t->kind == GSHL_TEMPLATE_HEXADECIMAL);

    if (buf == NULL) {
        t_mut->count = 0;

        usize remaining = t->hex;
        do {
            remaining >>= 4;
            t_mut->count += 1;
        } while (remaining != 0);

        return t->count;
    };

    static const char conversion[][17] = {
        [false] = "0123456789abcdef",
        [true] = "0123456789ABCDEF",
    };

    usize remaining = t->hex;
    usize count = 0;
    for (isize i = t->count - 1; i >= 0; --i) {
        buf[i] = conversion[t->opts.hex.uppercase][remaining % 16];
        remaining >>= 4;
        count += 1;
    }

    GSHL_ASSERT(count == t->count);

    return t->count;
}

#ifdef GSHL_TESTS


GSHL_TEST(write_hex)
{
#    define TEST_WRITE_HEX(NUMBER, EXPECTED_STRING, ...)                       \
        {                                                                      \
            char buffer[256] = {0};                                            \
            struct GSHL_Template template = {                                  \
                .kind = GSHL_TEMPLATE_HEXADECIMAL,                             \
                .hex = NUMBER,                                                 \
                .opts.hex = {__VA_ARGS__},                                     \
            };                                                                 \
            const usize count = GSHL_STACK_STRING_LEN(EXPECTED_STRING);        \
            GSHL_TEST_EQUAL(write_hex(NULL, &template), count);                \
            GSHL_TEST_EQUAL(write_hex(buffer, &template), count);              \
            GSHL_TEST_STR_EQUAL(buffer, EXPECTED_STRING);                      \
        }

    TEST_WRITE_HEX(0, "0");
    TEST_WRITE_HEX(0x0123456789abcde, "123456789abcde");
    TEST_WRITE_HEX(0x0123456789abcde, "123456789ABCDE", .uppercase = true);
    TEST_WRITE_HEX(0xabcdef0123456789, "abcdef0123456789", .uppercase = false);
    TEST_WRITE_HEX(0XABCDEF0123456789, "ABCDEF0123456789", .uppercase = true);

#    undef TEST_WRITE_HEX
}

#endif
#ifdef GSHL_SOURCE_CODE_MAPPING
#    line 2 "src/format/f64.c"
#endif // GSHL_SOURCE_CODE_MAPPING


#include <assert.h>
#include <stdlib.h>

usize write_f64(char *buf, GSHL_Template *t_mut)
{
    GSHL_UNUSED(buf);

    const GSHL_Template *const t = t_mut;

    GSHL_ASSERT(t->kind == GSHL_TEMPLATE_BINARY);

    GSHL_TODO("%s Not implemented yet", __FUNCTION__);

    return 0;
}
#ifdef GSHL_SOURCE_CODE_MAPPING
#    line 2 "src/format/f32.c"
#endif // GSHL_SOURCE_CODE_MAPPING


#include <assert.h>
#include <stdlib.h>

usize write_f32(char *buf, GSHL_Template *t_mut)
{
    GSHL_UNUSED(buf);

    const GSHL_Template *const t = t_mut;

    GSHL_ASSERT(t->kind == GSHL_TEMPLATE_BINARY);

    GSHL_TODO("%s Not implemented yet", __FUNCTION__);

    return 0;
}
#ifdef GSHL_SOURCE_CODE_MAPPING
#    line 4 "src/format/cstring.c"
#endif // GSHL_SOURCE_CODE_MAPPING

#include <assert.h>
#include <string.h>

usize write_cstring(char *buf, GSHL_Template *t_mut)
{
    const GSHL_Template *const t = t_mut;

    GSHL_ASSERT(t->kind == GSHL_TEMPLATE_CSTRING);

    if (buf == NULL) {
        return t_mut->count = strlen(t->cstring);
    }

    memcpy(buf, t->cstring, t->count);

    return t->count;
}

#ifdef GSHL_TESTS

GSHL_TEST(write_cstring)
{
#    define TEST_WRITE_CSTRING(CSTRING, EXPECTED_STRING, ...)                  \
        {                                                                      \
            char buffer[256] = {0};                                            \
            struct GSHL_Template template = {                                  \
                .kind = GSHL_TEMPLATE_CSTRING,                                 \
                .cstring = CSTRING,                                            \
                .opts.cstring = {__VA_ARGS__},                                 \
            };                                                                 \
            const usize count = GSHL_STACK_STRING_LEN(EXPECTED_STRING);        \
            GSHL_TEST_EQUAL(write_cstring(NULL, &template), count);            \
            GSHL_TEST_EQUAL(write_cstring(buffer, &template), count);          \
            GSHL_TEST_STR_EQUAL(buffer, EXPECTED_STRING);                      \
        }

    TEST_WRITE_CSTRING("", "");
    TEST_WRITE_CSTRING("abc", "abc");
    TEST_WRITE_CSTRING("   4l45krjslkac  \n", "   4l45krjslkac  \n");

#    undef TEST_WRITE_CSTRING
}

#endif
#ifdef GSHL_SOURCE_CODE_MAPPING
#    line 2 "src/format/char.c"
#endif // GSHL_SOURCE_CODE_MAPPING


#include <assert.h>
#include <string.h>

usize write_character(char *buf, GSHL_Template *t_mut)
{
    const GSHL_Template *const t = t_mut;

    GSHL_ASSERT(t->kind == GSHL_TEMPLATE_CHAR);

    if (buf == NULL) {
        return t_mut->count = t->character != '\0';
    }

    GSHL_ASSERT((t->character == '\0' && t->count == 0) ||
                (t->character != '\0' && t->count == 1));
    buf[0] = t->character;

    return t->count;
}

#ifdef GSHL_TESTS


GSHL_TEST(write_char)
{
#    define TEST_WRITE_CHAR(CHAR, EXPECTED_STRING, ...)                        \
        {                                                                      \
            char buffer[256] = {0};                                            \
            struct GSHL_Template template = {                                  \
                .kind = GSHL_TEMPLATE_CHAR,                                    \
                .character = CHAR,                                             \
                .opts.character = {__VA_ARGS__},                               \
            };                                                                 \
            const usize count = GSHL_STACK_STRING_LEN(EXPECTED_STRING);        \
            GSHL_TEST_EQUAL(write_character(NULL, &template), count);          \
            GSHL_TEST_EQUAL(write_character(buffer, &template), count);        \
            GSHL_TEST_STR_EQUAL(buffer, EXPECTED_STRING);                      \
        }

    TEST_WRITE_CHAR(0, "");
    TEST_WRITE_CHAR('0', "0");
    TEST_WRITE_CHAR('1', "1");
    TEST_WRITE_CHAR('2', "2");
    TEST_WRITE_CHAR('3', "3");
    TEST_WRITE_CHAR('4', "4");
    TEST_WRITE_CHAR('5', "5");
    TEST_WRITE_CHAR('6', "6");
    TEST_WRITE_CHAR('7', "7");
    TEST_WRITE_CHAR('8', "8");
    TEST_WRITE_CHAR('9', "9");

#    undef TEST_WRITE_CHAR
}

#endif
#ifdef GSHL_SOURCE_CODE_MAPPING
#    line 2 "src/format/bool.c"
#endif // GSHL_SOURCE_CODE_MAPPING


#include <assert.h>
#include <string.h>

usize write_boolean(char *buf, GSHL_Template *t_mut)
{
    const GSHL_Template *const t = t_mut;

    GSHL_ASSERT(t->kind == GSHL_TEMPLATE_BOOL);

    static const char *strings[] = {
        [false] = "false",
        [true] = "true",
    };

    static const usize lens[] = {
        [false] = GSHL_STACK_STRING_LEN("false"),
        [true] = GSHL_STACK_STRING_LEN("true"),
    };

    if (buf == NULL) {
        return t_mut->count = lens[t->boolean];
    }

    GSHL_ASSERT(t->count == lens[t->boolean]);

    memcpy(buf, strings[t->boolean], t->count);

    return t->count;
}

#ifdef GSHL_TESTS

GSHL_TEST(write_boolean)
{
#    define TEST_WRITE_BOOLEAN(NUMBER, EXPECTED_STRING, ...)                   \
        {                                                                      \
            char buffer[256] = {0};                                            \
            struct GSHL_Template template = {                                  \
                .kind = GSHL_TEMPLATE_BOOL,                                    \
                .boolean = NUMBER,                                             \
                .opts.boolean = {__VA_ARGS__},                                 \
            };                                                                 \
            const usize count = GSHL_STACK_STRING_LEN(EXPECTED_STRING);        \
            GSHL_TEST_EQUAL(write_boolean(NULL, &template), count);            \
            GSHL_TEST_EQUAL(write_boolean(buffer, &template), count);          \
            GSHL_TEST_STR_EQUAL(buffer, EXPECTED_STRING);                      \
        }

    TEST_WRITE_BOOLEAN(0, "false");
    TEST_WRITE_BOOLEAN(1, "true");
    TEST_WRITE_BOOLEAN(0b10101, "true");
    TEST_WRITE_BOOLEAN(~0L, "true");
    TEST_WRITE_BOOLEAN(true, "true");
    TEST_WRITE_BOOLEAN(false, "false");

#    undef TEST_WRITE_BOOLEAN
}

#endif
#ifdef GSHL_SOURCE_CODE_MAPPING
#    line 2 "src/format/binary.c"
#endif // GSHL_SOURCE_CODE_MAPPING


#include <assert.h>
#include <stdlib.h>

usize write_binary(char *buf, GSHL_Template *t_mut)
{
    const GSHL_Template *const t = t_mut;

    GSHL_ASSERT(t->kind == GSHL_TEMPLATE_BINARY);

    if (buf == NULL) {
        t_mut->count = 0;

        u64 remaining = t->binary;

        do {
            t_mut->count += 1;
            remaining >>= 1;
        } while (remaining != 0);

        return t->count;
    }

    usize count = 0;
    u64 remaining = t->binary;
    do {
        count += 1;

        buf[t->count - count] = "01"[remaining & 1];

        remaining >>= 1;
    } while (remaining != 0);

    GSHL_ASSERT(count == t->count);

    return count;
}

#ifdef GSHL_TESTS

GSHL_TEST(write_binary)
{
#    define TEST_WRITE_BINARY(NUMBER, EXPECTED_STRING, ...)                    \
        {                                                                      \
            char buffer[256] = {0};                                            \
            struct GSHL_Template template = {                                  \
                .kind = GSHL_TEMPLATE_BINARY,                                  \
                .binary = NUMBER,                                              \
                .opts.binary = {__VA_ARGS__},                                  \
            };                                                                 \
            const usize count = GSHL_STACK_STRING_LEN(EXPECTED_STRING);        \
            GSHL_TEST_EQUAL(write_binary(NULL, &template), count);             \
            GSHL_TEST_EQUAL(write_binary(buffer, &template), count);           \
            GSHL_TEST_STR_EQUAL(buffer, EXPECTED_STRING);                      \
        }

    TEST_WRITE_BINARY(0, "0");
    TEST_WRITE_BINARY(0b10101, "10101");
    TEST_WRITE_BINARY(1 << 5, "100000");
    TEST_WRITE_BINARY(
        ~0L,
        "1111111111111111111111111111111111111111111111111111111111111111");
    TEST_WRITE_BINARY(0xFF, "11111111");
#    undef TEST_WRITE_BINARY
}

#endif
#ifdef GSHL_SOURCE_CODE_MAPPING
#    line 4 "src/array/mod.c"
#endif // GSHL_SOURCE_CODE_MAPPING
#include <stdlib.h>

#ifdef GSHL_TESTS


GSHL_TEST(stackarray)
{
    const u8 a[] = {1, 2, 3, 4, 5};

    GSHL_TEST_EQUAL(GSHL_ARRAY_LEN(a), 5);

    u8 sum = 0;
    GSHL_ARRAY_FOREACH(a, u8 element) { sum += element; }
    GSHL_TEST_EQUAL(sum, 15);
}

GSHL_TEST(array)
{
    const struct {
        i32 *items;
        usize count;
    } array = {
        .count = 5,
        .items = calloc(5, sizeof(i32)),
    };

    GSHL_ARRAYN_FOREACH_MUT(array.items, array.count, i32 * element)
    {
        *element = index + 1;
    }

    u8 sum = 0;
    GSHL_ARRAYN_FOREACH(array.items, array.count, i32 element)
    {
        sum += element;
    }
    GSHL_TEST_EQUAL(sum, 15);

    free(array.items);
}

#endif
#endif // GSHL_IMPLEMENTATION
