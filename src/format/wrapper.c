#include "format/wrapper.h"

#include "format/binary.h"  // IWYU pragma: keep
#include "format/bool.h"    // IWYU pragma: keep
#include "format/char.h"    // IWYU pragma: keep
#include "format/cstring.h" // IWYU pragma: keep
#include "format/f32.h"     // IWYU pragma: keep
#include "format/f64.h"     // IWYU pragma: keep
#include "format/hex.h"     // IWYU pragma: keep
#include "format/i16.h"     // IWYU pragma: keep
#include "format/i32.h"     // IWYU pragma: keep
#include "format/i64.h"     // IWYU pragma: keep
#include "format/i8.h"      // IWYU pragma: keep
#include "format/isize.h"   // IWYU pragma: keep
#include "format/pointer.h" // IWYU pragma: keep
#include "format/u16.h"     // IWYU pragma: keep
#include "format/u32.h"     // IWYU pragma: keep
#include "format/u64.h"     // IWYU pragma: keep
#include "format/u8.h"      // IWYU pragma: keep
#include "format/usize.h"   // IWYU pragma: keep

#include "array/mod.h"
#include "macros/mod.h"
#include "types/mod.h"

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
