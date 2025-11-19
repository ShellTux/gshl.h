#ifndef INCLUDE_INCLUDE_MACROS_H_
#define INCLUDE_INCLUDE_MACROS_H_

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

#ifdef GSHL_STRIP_PREFIX
#    define loop GSHL_LOOP
#    define LOOP GSHL_LOOP
#    define TODO GSHL_TODO
#    define UNREACHABLE GSHL_UNREACHABLE
#    define UNUSED GSHL_UNUSED
#endif

#endif // INCLUDE_INCLUDE_MACROS_H_
#ifndef INCLUDE_TYPES_MOD_H_
#define INCLUDE_TYPES_MOD_H_

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

#endif // INCLUDE_TYPES_MOD_H_
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

#define F32_MAX 340282346638528859811704183484516925440.0000000000000000f
#define F32_MIN -340282346638528859811704183484516925440.0000000000000000f

#define F64_MAX                                                                \
    179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.0000000000000000
#define F64_MIN                                                                \
    -179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.0000000000000000

/// }}}

#endif // INCLUDE_TYPES_LIMITS_H_
#ifndef INCLUDE_INCLUDE_MATH_H_
#define INCLUDE_INCLUDE_MATH_H_


usize GSHL_factorial(const usize number);

#endif // INCLUDE_INCLUDE_MATH_H_
#ifndef INCLUDE_FORMAT_U32_H_
#define INCLUDE_FORMAT_U32_H_


/// {{{ Functions

usize write_u32(char *buf, const u32 u32, const usize precomputed_count);

/// }}}

#endif // INCLUDE_FORMAT_U32_H_
// vim: foldmethod=marker
#ifndef INCLUDE_INCLUDE_FORMAT_H_
#define INCLUDE_INCLUDE_FORMAT_H_


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
#ifndef INCLUDE_FORMAT_POINTER_H_
#define INCLUDE_FORMAT_POINTER_H_


/// {{{ Functions

usize write_pointer(char *buf, const void *pointer,
                    const usize precomputed_count);

/// }}}

#endif // INCLUDE_FORMAT_POINTER_H_
#ifndef INCLUDE_FORMAT_I32_H_
#define INCLUDE_FORMAT_I32_H_


/// {{{ Functions

usize write_i32(char *buf, const i32 i32, const usize precomputed_count);

/// }}}

#endif // INCLUDE_FORMAT_I32_H_
#ifndef INCLUDE_FORMAT_HEX_H_
#define INCLUDE_FORMAT_HEX_H_


/// {{{ Functions

usize write_hex(char *buf, const usize hex, const usize precomputed_count);

/// }}}

#endif // INCLUDE_FORMAT_HEX_H_
#ifndef INCLUDE_FORMAT_CHAR_H_
#define INCLUDE_FORMAT_CHAR_H_


/// {{{ Functions

usize write_char(char *buf, const char character,
                 const usize precomputed_count);

/// }}}

#endif // INCLUDE_FORMAT_CHAR_H_
#ifndef INCLUDE_FORMAT_CSTRING_H_
#define INCLUDE_FORMAT_CSTRING_H_


/// {{{ Functions

usize write_cstring(char *buf, const char *cstring,
                    const usize precomputed_count);

/// }}}

#endif // INCLUDE_FORMAT_CSTRING_H_
// vim: foldmethod=marker
#ifndef INCLUDE_INCLUDE_TEST_H_
#define INCLUDE_INCLUDE_TEST_H_

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
            fprintf(stderr,                                                    \
                    " %s:%02d " GSHL_FG_CYAN(" %-30s ") " ... " GSHL_FG_RED(   \
                        " FAILED ") " Assertion failed : \"%s\" == \"%s\"\n",  \
                    __FILE__, __LINE__, __func__, STR1, STR2);                 \
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

typedef struct {
    bool continue_on_fail;
} GSHL_TestEqualOpt;

/// }}}

/// {{{ Functions

GSHLDEF void GSHL_register_test_wrapper(const char *name,
                                        GSHL_TestFunction test_func);
GSHLDEF usize GSHL_run_tests(const char *filter);

/// }}}

/// {{{ Stripping

#ifdef GSHL_STRIP_PREFIX
#    define TEST_EQUAL GSHL_TEST_EQUAL
#    define REGISTER_TEST GSHL_REGISTER_TEST
#    define run_tests GSHL_run_tests
#endif

/// }}}

#endif // INCLUDE_INCLUDE_TEST_H_
#ifndef INCLUDE_INCLUDE_ANSI_H_
#define INCLUDE_INCLUDE_ANSI_H_


#endif // INCLUDE_INCLUDE_ANSI_H_
#ifndef INCLUDE_ANSI_COLORS_H_
#define INCLUDE_ANSI_COLORS_H_

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

#endif // INCLUDE_ANSI_COLORS_H_
#ifndef INCLUDE_INCLUDE_ARRAY_H_
#define INCLUDE_INCLUDE_ARRAY_H_


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

#endif // INCLUDE_INCLUDE_ARRAY_H_
#ifdef GSHL_IMPLEMENTATION
#ifdef GSHL_SOURCE_CODE_MAPPING
#    line 2 "src/types/limits.c"
#endif // GSHL_SOURCE_CODE_MAPPING

#ifdef GSHL_TESTS
#    include "test/mod.h"

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
#    line 2 "src/types/mod.c"
#endif // GSHL_SOURCE_CODE_MAPPING

#ifdef GSHL_TESTS

#    include "test/mod.h"

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

#    include "test/mod.h"

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
#    line 2 "src/format/u32.c"
#endif // GSHL_SOURCE_CODE_MAPPING


#include <assert.h>
#include <string.h>

usize write_u32(char *buf, const u32 value, const usize precomputed_count)
{
    if (buf == NULL || precomputed_count == 0) {
        usize u32_count = 0;

        u32 remaining = value;
        do {
            remaining /= 10;
            u32_count += 1;
        } while (remaining != 0);

        return u32_count;
    }

    if (value == 0) {
        buf[0] = '0';
        return precomputed_count;
    }

    u32 remaining = value;
    for (isize i = precomputed_count - 1; i >= 0 && remaining > 0;
         --i, remaining /= 10) {
        buf[i] = '0' + (remaining % 10);
    }

    return precomputed_count;
}

#ifdef GSHL_TESTS

#    include "test/mod.h"

GSHL_TEST(write_u32)
{
#    define TEST_WRITE_U32(NUMBER, EXPECTED_COUNT, EXPECTED_STRING)            \
        {                                                                      \
            char buffer[256] = {0};                                            \
            const usize count = write_u32(NULL, NUMBER, 0);                    \
            GSHL_TEST_EQUAL(count, EXPECTED_COUNT);                            \
            GSHL_TEST_EQUAL(write_u32(buffer, NUMBER, count), EXPECTED_COUNT); \
            GSHL_TEST_STR_EQUAL(buffer, EXPECTED_STRING);                      \
        }

    TEST_WRITE_U32(0, 1, "0");
    TEST_WRITE_U32(3245, 4, "3245");
    TEST_WRITE_U32(34948239, 8, "34948239");
    TEST_WRITE_U32(U32_MAX, 10, "4294967295");
    TEST_WRITE_U32(U32_MIN, 1, "0");

#    undef TEST_WRITE_U32
}

#endif
#ifdef GSHL_SOURCE_CODE_MAPPING
#    line 2 "src/format/hex.c"
#endif // GSHL_SOURCE_CODE_MAPPING

#include <string.h>

usize write_hex(char *buf, const usize hex, const usize precomputed_count)
{
    static const char prefix[] = "0x";
    static const usize prefixLen = sizeof(prefix) - 1;

    if (buf == NULL || precomputed_count == 0) {
        usize count = 0;
        usize remaining = hex;
        do {
            remaining >>= 4;
            count += 1;
        } while (remaining != 0);

        return prefixLen + count;
    }

    usize remaining = hex;
    for (isize i = precomputed_count - prefixLen - 1; i >= 0; --i) {
        buf[prefixLen + i] = "0123456789ABCDEF"[remaining % 16];
        remaining >>= 4;
    }

    memcpy(buf, prefix, prefixLen);

    return precomputed_count;
}
#ifdef GSHL_SOURCE_CODE_MAPPING
#    line 6 "src/format/mod.c"
#endif // GSHL_SOURCE_CODE_MAPPING


#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

void GSHL_free_FormatWrapperResult(struct GSHL_FormatWrapperResult *result)
{
    free(result->string);
    if (result->templates.items != NULL) {
        free(result->templates.items);
    }

    *result = (struct GSHL_FormatWrapperResult){0};
}

GSHL_FormatWrapperResult GSHL_format_wrapper(const char *restrict format, ...)
{
    va_list args;
    va_start(args, format);

    const GSHL_FormatWrapperResult result = GSHL_format_wrapperv(format, args);

    va_end(args);

    return result;
}

GSHL_FormatWrapperResult GSHL_format_wrapperv(const char *restrict format,
                                              va_list args)
{
    GSHL_FormatWrapperResult result = {0};

    if (format[0] == '\0') {
        result.string = calloc(1, *format);
        return result;
    }

    char *formatEnd = NULL;

    /*
     * Count how many template specifiers
     * Count how many non-template bytes into result.count
     */
    for (char *formatC = (char *)format; *formatC != '\0';
         formatEnd = (formatC += 1) + 1) {
        if (*formatC == '%') {
            switch (formatC[1]) {
            case 'i':
            case 's':
            case 'u':
            case 'c':
            case 'f':
            case 'p':
            case '%':
            case 'x':
                result.templates.count += 1;
                break;
            default:
                GSHL_UNREACHABLE(
                    "Unknown template specifier '%c', format: \"%s\"",
                    formatC[1], format);
            }

            formatC++;

            continue;
        }

        result.count += 1;
    }

    /*
     * Fill templates
     */
    if (result.templates.count != 0) {
        result.templates.items =
            calloc(result.templates.count, sizeof(*result.templates.items));

        usize template_index = 0;

        for (char *formatC = (char *)format; *formatC != '\0';
             formatEnd = (formatC += 1) + 1) {
            if (formatC[0] == '%') {
                GSHL_Template *t = &result.templates.items[template_index++];

                *t = (GSHL_Template){0};
                t->formatStart = &formatC[0];
                t->formatEnd = &formatC[2];
                t->count = 0;

                switch (formatC[1]) {
                case 'i': {
                    t->kind = GSHL_TEMPLATE_I32;
                    t->i32 = va_arg(args, i32);
                    t->write.i32 = write_i32;
                    t->count = t->write.i32(NULL, t->i32, 0);
                } break;

                case 'u': {
                    t->kind = GSHL_TEMPLATE_U32;
                    t->i32 = va_arg(args, u32);
                    t->write.u32 = write_u32;
                    t->count = t->write.u32(NULL, t->u32, 0);
                } break;

                case 'c': {
                    t->kind = GSHL_TEMPLATE_CHAR;
                    t->character = va_arg(args, int);
                    t->write.character = write_char;
                    t->count = t->write.character(NULL, t->character, 0);
                } break;

                case 's': {
                    t->kind = GSHL_TEMPLATE_CSTRING;
                    t->cstring = va_arg(args, char *);
                    t->write.cstring = write_cstring;
                    t->count = t->write.cstring(NULL, t->cstring, 0);
                } break;

                case 'p': {
                    t->kind = GSHL_TEMPLATE_POINTER;
                    t->pointer = va_arg(args, void *);
                    t->write.pointer = write_pointer;
                    t->count = t->write.pointer(NULL, t->pointer, 0);
                } break;

                case '%': {
                    t->kind = GSHL_TEMPLATE_CHAR;
                    t->character = '%';
                    t->write.character = write_char;
                    t->count = t->write.character(NULL, t->character, 0);
                } break;

                case 'x': {
                    t->kind = GSHL_TEMPLATE_HEXADECIMAL;
                    t->hex = va_arg(args, u32);
                    t->write.hex = write_hex;
                    t->count = t->write.hex(NULL, t->hex, 0);
                } break;

                default:
                    GSHL_UNREACHABLE(
                        "Template not implemented yet '%c', format: \"%s\"",
                        formatC[1], format);
                }

                result.count += t->count;
                formatC++;

                continue;
            }
        }
    }

    // GSHL_ARRAYN_FOREACH(result.templates.items, result.templates.count,
    //                     GSHL_Template template)
    // {
    //     fprintf(stderr,
    //             "{\n"
    //             "  formatStart = (%p) %s\n"
    //             "  formatEnd   = (%p) %s\n"
    //             "}\n",
    //             template.formatStart, template.formatStart,
    //             template.formatEnd, template.formatEnd);
    // }

    result.string = calloc(result.count + 1, sizeof(*result.string));
    // fprintf(stderr, "count = %zu\n", result.count);

    {
        char *formatStart = (char *)format;
        char *output = result.string;
        GSHL_ARRAYN_FOREACH(result.templates.items, result.templates.count,
                            GSHL_Template template)
        {
            const usize len = template.formatStart - formatStart;
            memcpy(output, formatStart, len);
            output += len;

            // fprintf(stderr, "template.formatStart (%p) = %s\n",
            //         template.formatStart, template.formatStart);
            // fprintf(stderr, "formatStart (%p) = %s\n", formatStart,
            //         formatStart);
            // fprintf(stderr, "len = %zu\n", len);
            // fprintf(stderr, "i32 = %i\n", template.i32);
            // fprintf(stderr, "string (%p) = %s\n", result.string,
            // result.string); fprintf(stderr, "output (%p) = %s\n", output,
            // output);

            switch (template.kind) {
            case GSHL_TEMPLATE_CSTRING:
                output += template.write.cstring(output, template.cstring,
                                                 template.count);
                break;
            case GSHL_TEMPLATE_CHAR:
                output += template.write.character(output, template.character,
                                                   template.count);
                break;
            case GSHL_TEMPLATE_I32:
                output +=
                    template.write.i32(output, template.i32, template.count);
                break;
            case GSHL_TEMPLATE_U32:
                output +=
                    template.write.u32(output, template.u32, template.count);
                break;
            case GSHL_TEMPLATE_F32:
                output +=
                    template.write.f32(output, template.f32, template.count);
                break;
            case GSHL_TEMPLATE_POINTER:
                output += template.write.pointer(output, template.pointer,
                                                 template.count);
                break;
            case GSHL_TEMPLATE_HEXADECIMAL:
                output +=
                    template.write.hex(output, template.hex, template.count);
                break;
            default:
                GSHL_UNREACHABLE(
                    "Template write not implemented yet '%.*s', format: \"%s\"",
                    (int)(template.formatEnd - template.formatStart),
                    template.formatStart, format);
                break;
            }

            formatStart = template.formatEnd;
        }

        memcpy(output, formatStart, formatEnd - formatStart);
    }

    return result;
}

#ifdef GSHL_TESTS

#    include "test/mod.h"

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
                (void)templates;                                               \
                ADDITIONAL_TESTS                                               \
            } while (0);                                                       \
            GSHL_free_FormatWrapperResult(&result);                            \
        }

    TEST_FORMAT_WRAPPER_RESULT("", 0, , "");
    TEST_FORMAT_WRAPPER_RESULT("Hello world!\n", 13, , "Hello world!\n");
    TEST_FORMAT_WRAPPER_RESULT(
        "3245", 4,
        {
            GSHL_TEST_EQUAL(result.templates.count, 1);
            GSHL_TEST_EQUAL(templates[0].kind, GSHL_TEMPLATE_I32);
            GSHL_TEST_EQUAL(templates[0].i32, 3245);
        },
        "%i", 3245);
    TEST_FORMAT_WRAPPER_RESULT(
        "1 -2 3", 6,
        {
            GSHL_TEST_EQUAL(result.templates.count, 3);
            GSHL_TEST_EQUAL(templates[0].kind, GSHL_TEMPLATE_I32);
            GSHL_TEST_EQUAL(templates[0].i32, 1);
            GSHL_TEST_EQUAL(templates[1].kind, GSHL_TEMPLATE_I32);
            GSHL_TEST_EQUAL(templates[1].i32, -2);
            GSHL_TEST_EQUAL(templates[2].kind, GSHL_TEMPLATE_I32);
            GSHL_TEST_EQUAL(templates[2].i32, 3);
        },
        "%i %i %i", 1, -2, 3);

    TEST_FORMAT_WRAPPER_RESULT(
        "    -485   ", 11,
        {
            GSHL_TEST_EQUAL(result.templates.count, 1);
            GSHL_TEST_EQUAL(templates[0].kind, GSHL_TEMPLATE_I32);
            GSHL_TEST_EQUAL(templates[0].i32, -485);
        },
        "    %i   ", -485);

    TEST_FORMAT_WRAPPER_RESULT(
        "Hello world!\n", 13,
        {
            GSHL_TEST_EQUAL(result.templates.count, 1);
            GSHL_TEST_EQUAL(templates[0].kind, GSHL_TEMPLATE_CSTRING);
            GSHL_TEST_STR_EQUAL(templates[0].cstring, "Hello world!");
        },
        "%s\n", "Hello world!");

    TEST_FORMAT_WRAPPER_RESULT(
        "  Foo  bar  baz ", 16,
        {
            GSHL_TEST_EQUAL(result.templates.count, 3);
            GSHL_TEST_EQUAL(templates[0].kind, GSHL_TEMPLATE_CSTRING);
            GSHL_TEST_STR_EQUAL(templates[0].cstring, "Foo");
            GSHL_TEST_EQUAL(templates[1].kind, GSHL_TEMPLATE_CSTRING);
            GSHL_TEST_STR_EQUAL(templates[1].cstring, "bar");
            GSHL_TEST_EQUAL(templates[2].kind, GSHL_TEMPLATE_CSTRING);
            GSHL_TEST_STR_EQUAL(templates[2].cstring, "baz");
        },
        "  %s  %s  %s ", "Foo", "bar", "baz");

    TEST_FORMAT_WRAPPER_RESULT(
        "count = 5", 9,
        {
            GSHL_TEST_EQUAL(result.templates.count, 2);
            GSHL_TEST_EQUAL(templates[0].kind, GSHL_TEMPLATE_CSTRING);
            GSHL_TEST_STR_EQUAL(templates[0].cstring, "count");
            GSHL_TEST_EQUAL(templates[1].kind, GSHL_TEMPLATE_I32);
            GSHL_TEST_EQUAL(templates[1].i32, 5);
        },
        "%s = %i", "count", 5);

    TEST_FORMAT_WRAPPER_RESULT(
        "i32: 42", 7,
        {
            GSHL_TEST_EQUAL(result.templates.count, 1);
            GSHL_TEST_EQUAL(templates[0].kind, GSHL_TEMPLATE_I32);
            GSHL_TEST_EQUAL(templates[0].i32, 42);
        },
        "i32: %i", 42)

    TEST_FORMAT_WRAPPER_RESULT(
        "u32: 42", 7,
        {
            GSHL_TEST_EQUAL(result.templates.count, 1);
            GSHL_TEST_EQUAL(templates[0].kind, GSHL_TEMPLATE_U32);
            GSHL_TEST_EQUAL(templates[0].u32, 42);
        },
        "u32: %u", 42)

    TEST_FORMAT_WRAPPER_RESULT(
        "char: F", 7,
        {
            GSHL_TEST_EQUAL(result.templates.count, 1);
            GSHL_TEST_EQUAL(templates[0].kind, GSHL_TEMPLATE_CHAR);
            GSHL_TEST_EQUAL(templates[0].character, 'F');
        },
        "char: %c", 'F')

    TEST_FORMAT_WRAPPER_RESULT(
        "void*: nil", 10,
        {
            GSHL_TEST_EQUAL(result.templates.count, 1);
            GSHL_TEST_EQUAL(templates[0].kind, GSHL_TEMPLATE_POINTER);
            GSHL_TEST_EQUAL(templates[0].pointer, NULL);
        },
        "void*: %p", NULL)

    TEST_FORMAT_WRAPPER_RESULT(
        "void*: 0x7ffc0d1b64f4", 21,
        {
            GSHL_TEST_EQUAL(result.templates.count, 1);
            GSHL_TEST_EQUAL(templates[0].kind, GSHL_TEMPLATE_POINTER);
            GSHL_TEST_EQUAL(templates[0].pointer, (void *)0x7ffc0d1b64f4);
        },
        "void*: %p", (void *)0x7ffc0d1b64f4)

    TEST_FORMAT_WRAPPER_RESULT(
        "%", 1,
        {
            GSHL_TEST_EQUAL(result.templates.count, 1);
            GSHL_TEST_EQUAL(templates[0].kind, GSHL_TEMPLATE_CHAR);
            GSHL_TEST_EQUAL(templates[0].character, '%');
        },
        "%%")

    TEST_FORMAT_WRAPPER_RESULT(
        "0x69", 4,
        {
            GSHL_TEST_EQUAL(result.templates.count, 1);
            GSHL_TEST_EQUAL(templates[0].kind, GSHL_TEMPLATE_HEXADECIMAL);
            GSHL_TEST_EQUAL(templates[0].hex, 105UL);
        },
        "%x", 105UL)

    TEST_FORMAT_WRAPPER_RESULT(
        "0x69 24", 7,
        {
            GSHL_TEST_EQUAL(result.templates.count, 2);
            GSHL_TEST_EQUAL(templates[0].kind, GSHL_TEMPLATE_HEXADECIMAL);
            GSHL_TEST_EQUAL(templates[0].hex, 105UL);
            GSHL_TEST_EQUAL(templates[1].kind, GSHL_TEMPLATE_I32);
            GSHL_TEST_EQUAL(templates[1].i32, 24);
        },
        "%x %i", 105, 24)

#    undef TEST_FORMAT_WRAPPER_RESULT
}

#endif // GSHL_TEST
#ifdef GSHL_SOURCE_CODE_MAPPING
#    line 2 "src/format/cstring.c"
#endif // GSHL_SOURCE_CODE_MAPPING

#include <string.h>

usize write_cstring(char *buf, const char *cstring,
                    const usize precomputed_count)
{
    if (buf == NULL || precomputed_count == 0) {
        return strlen(cstring);
    }

    memcpy(buf, cstring, precomputed_count);

    return precomputed_count;
}

#ifdef GSHL_TESTS
#    include "test/mod.h"

GSHL_TEST(write_cstring)
{
#    define TEST_WRITE_CSTRING(CSTRING, EXPECTED_COUNT, EXPECTED_STRING)       \
        {                                                                      \
            char buffer[256] = {0};                                            \
            const usize count = write_cstring(NULL, CSTRING, 0);               \
            GSHL_TEST_EQUAL(count, EXPECTED_COUNT);                            \
            GSHL_TEST_EQUAL(write_cstring(buffer, CSTRING, count),             \
                            EXPECTED_COUNT);                                   \
            GSHL_TEST_STR_EQUAL(buffer, EXPECTED_STRING);                      \
        }

    TEST_WRITE_CSTRING("", 0, "");
    TEST_WRITE_CSTRING("abc", 3, "abc");
    TEST_WRITE_CSTRING("   4l45krjslkac  \n", 18, "   4l45krjslkac  \n");

#    undef TEST_WRITE_CSTRING
}

#endif
#ifdef GSHL_SOURCE_CODE_MAPPING
#    line 2 "src/format/pointer.c"
#endif // GSHL_SOURCE_CODE_MAPPING

#include <assert.h>
#include <string.h>

usize write_pointer(char *buf, const void *pointer,
                    const usize precomputed_count)
{
    static const char prefix[] = "0x";
    static const usize prefixLen = sizeof(prefix) - 1;

    if (buf == NULL || precomputed_count == 0) {
        if (pointer == NULL) {
            return sizeof("nil") - 1;
        }

        usize count = 0;
        uintptr_t address = (uintptr_t)pointer;
        do {
            address >>= 4;
            count += 1;
        } while (address != 0);

        return prefixLen + count; // two hex digits for each byte
    }

    if (pointer == NULL) {
        assert(precomputed_count == 3);
        memcpy(buf, "nil", 3);
        return precomputed_count;
    }

    uintptr_t address = (uintptr_t)pointer;

    // Fill the buffer with hexadecimal representation
    for (isize i = precomputed_count - prefixLen - 1; i >= 0 && address > 0;
         --i, address >>= 4) {
        buf[prefixLen + i] = "0123456789abcdef"[address % 16]; // Lowercase hex
    }

    memcpy(buf, prefix, prefixLen);

    return precomputed_count;
}
#ifdef GSHL_SOURCE_CODE_MAPPING
#    line 2 "src/format/char.c"
#endif // GSHL_SOURCE_CODE_MAPPING


#include <assert.h>
#include <string.h>

usize write_char(char *buf, const char character, const usize precomputed_count)
{
    if (buf == NULL || precomputed_count == 0) {
        return 1;
    }

    assert(precomputed_count == 1);
    buf[0] = character;

    return precomputed_count;
}

#ifdef GSHL_TESTS

#    include "test/mod.h"

GSHL_TEST(write_char)
{
#    define TEST_WRITE_CHAR(CHAR, EXPECTED_COUNT, EXPECTED_STRING)             \
        {                                                                      \
            char buffer[256] = {0};                                            \
            const usize count = write_char(NULL, CHAR, 0);                     \
            GSHL_TEST_EQUAL(count, EXPECTED_COUNT);                            \
            GSHL_TEST_EQUAL(write_char(buffer, CHAR, count), EXPECTED_COUNT);  \
            GSHL_TEST_STR_EQUAL(buffer, EXPECTED_STRING);                      \
        }

    TEST_WRITE_CHAR(0, 1, "");
    TEST_WRITE_CHAR('0', 1, "0");
    TEST_WRITE_CHAR('1', 1, "1");
    TEST_WRITE_CHAR('2', 1, "2");
    TEST_WRITE_CHAR('3', 1, "3");
    TEST_WRITE_CHAR('4', 1, "4");
    TEST_WRITE_CHAR('5', 1, "5");
    TEST_WRITE_CHAR('6', 1, "6");
    TEST_WRITE_CHAR('7', 1, "7");
    TEST_WRITE_CHAR('8', 1, "8");
    TEST_WRITE_CHAR('9', 1, "9");

#    undef TEST_WRITE_CHAR
}

#endif
#ifdef GSHL_SOURCE_CODE_MAPPING
#    line 4 "src/format/i32.c"
#endif // GSHL_SOURCE_CODE_MAPPING

#include <assert.h>
#include <string.h>

usize write_i32(char *buf, const i32 value, const usize precomputed_count)
{
    if (buf == NULL || precomputed_count == 0) {
        usize i32_count = 0;

        i32 remaining = value;
        do {
            remaining /= 10;
            i32_count += 1;
        } while (remaining != 0);

        return (value >= 0 ? sizeof("") - 1 : sizeof("-") - 1) + i32_count;
    }

    const usize count = precomputed_count;
    if (value < 0) {
        buf[0] = '-';
    }
    else if (value == 0) {
        buf[0] = '0';
        return count;
    }

    i32 remaining = value;
    if (remaining < 0) {
        buf[0] = '-';
        remaining *= -1;
    }

    for (isize i = count - 1; i >= 0 && remaining > 0; --i, remaining /= 10) {
        buf[i] = '0' + (remaining % 10);
    }

    return count;
}

#ifdef GSHL_TESTS

#    include "test/mod.h"

GSHL_TEST(write_i32)
{
    GSHL_TEST_EQUAL(write_i32(NULL, 0, 0), 1);
    GSHL_TEST_EQUAL(write_i32(NULL, 34948239, 0), 8);
    GSHL_TEST_EQUAL(write_i32(NULL, -0, 0), 1);
    GSHL_TEST_EQUAL(write_i32(NULL, -5, 0), 2);
    GSHL_TEST_EQUAL(write_i32(NULL, -534, 0), 4);
    GSHL_TEST_EQUAL(write_i32(NULL, I32_MAX, 0), 10);
    GSHL_TEST_EQUAL(write_i32(NULL, I32_MIN, 0), 11);

#    define TEST_WRITE_I32(NUMBER, EXPECTED_COUNT, EXPECTED_STRING)            \
        {                                                                      \
            char buffer[256] = {0};                                            \
            const usize count = write_i32(NULL, NUMBER, 0);                    \
            GSHL_TEST_EQUAL(count, EXPECTED_COUNT);                            \
            GSHL_TEST_EQUAL(write_i32(buffer, NUMBER, count), EXPECTED_COUNT); \
            GSHL_TEST_STR_EQUAL(buffer, EXPECTED_STRING);                      \
        }

    TEST_WRITE_I32(0, 1, "0");
    TEST_WRITE_I32(3245, 4, "3245");
    TEST_WRITE_I32(34948239, 8, "34948239");
    TEST_WRITE_I32(-0, 1, "0");
    TEST_WRITE_I32(-5, 2, "-5");
    TEST_WRITE_I32(-534, 4, "-534");
    TEST_WRITE_I32(I32_MAX, 10, "2147483647");
    {
        // GSHL_TEST_EQUAL(write_i32(buffer, I32_MIN), 11);
        // GSHL_TEST_STR_EQUAL(buffer, "");
    }
#    undef TEST_WRITE_I32
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
#    line 4 "src/array/mod.c"
#endif // GSHL_SOURCE_CODE_MAPPING
#include <stdlib.h>

#ifdef GSHL_TESTS

#    include "test/mod.h"

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
