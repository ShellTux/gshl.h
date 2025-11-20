#include "format/mod.h"
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
#define KIND(ENUM, NAME, TYPE, VA_ARG_TYPE, BIT_SHIFT, ...)                    \
    {GSHL_TEMPLATE_##ENUM, {__VA_ARGS__}},
        GHSL_TEMPLATE_KINDS
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
        assert(result.string != NULL);
        return result;
    }

    char *formatEnd = NULL;

    /*
     * Count how many template specifiers
     * Count how many non-template bytes into result.count
     */
    isize parenBalance = 0;
    for (formatEnd = (char *)format; *formatEnd != '\0'; formatEnd += 1) {
        const char *formatC = formatEnd;
        /*
         * Backward compatible format specifiers
         */
        if (formatC[0] == '%') {
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
                formatEnd = (char *)formatC + 1;
                break;
            default:
                GSHL_UNREACHABLE(
                    "Unknown template specifier '%c', format: \"%s\"",
                    formatC[1], format);
            }
        }
        else if ((formatC[0] == '}' && formatC[1] == '}') ||
                 (formatC[0] == '{' && formatC[1] == '{')) {

            formatEnd = (char *)formatC + 1;
            result.templates.count += 1;
        }
        else if (formatC[0] == '{') {

            parenBalance += 1;

            for (; formatC[0] != '\0' && formatC[1] != '}'; formatC += 1) {
            }

            assert(formatC[0] != '\0');
            assert(formatC[1] == '}');

            parenBalance -= 1;

            formatEnd = (char *)formatC + 1;
            result.templates.count += 1;
        }
        else {
            result.count += 1;
        }
    }

    GSHL_FORMAT_DEBUG_PRINT("formatEnd = (%p)\n", formatEnd);
    GSHL_FORMAT_DEBUG_PRINT("formatEnd - formatStart = %li\n",
                            formatEnd - format);
    GSHL_FORMAT_DEBUG_PRINT("len(format) = %lu\n", strlen(format));

    assert(formatEnd - format == (isize)strlen(format));
    assert(parenBalance == 0);

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
                    t->write.character = write_character;
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
                    t->write.character = write_character;
                    t->count = t->write.character(NULL, t->character, 0);
                } break;

                case 'x': {
                    t->kind = GSHL_TEMPLATE_HEXADECIMAL;
                    t->hex = va_arg(args, u32);
                    t->write.hex = write_hex;
                    t->count = t->write.hex(NULL, t->hex, 0);
                } break;

                case 'l': {
                    t->formatEnd = formatC + 3;
                    GSHL_TODO("Template l not implemented yet!");

                    switch (formatC[2]) {
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
                t->count = t->write.character(NULL, t->character, 0);

                result.count += t->count;

                formatC = t->formatEnd - 1;
                template_index += 1;
            }
            else if (formatC[0] == '{') {
                t->formatStart = formatC;

                t->kind = GSHL_template_kind_from_str(&formatC);
                switch (t->kind) {
#define KIND(ENUM, NAME, TYPE, VA_ARG_TYPE, BIT_SHIFT, ...)                    \
    case GSHL_TEMPLATE_##ENUM:                                                 \
        t->write.NAME = write_##NAME;                                          \
        t->NAME = va_arg(args, VA_ARG_TYPE);                                   \
        t->count = t->write.NAME(NULL, t->NAME, 0);                            \
        break;
                    GHSL_TEMPLATE_KINDS
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

#if GSHL_FORMAT_DEBUG
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
        GSHL_FORMAT_DEBUG_PRINT(" result.string = \"%s\"\n", result.string);
        GSHL_ARRAYN_FOREACH(result.templates.items, result.templates.count,
                            GSHL_Template template)
        {
            GSHL_ASSERT(formatStart <= template.formatStart);
            GSHL_ASSERT(formatStart <= template.formatEnd &&
                        template.formatEnd <= formatEnd);

            const usize len = template.formatStart - formatStart;
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

            switch (template.kind) {
#define KIND(ENUM, NAME, TYPE, VA_ARG_TYPE, BIT_SHIFT, ...)                    \
    case GSHL_TEMPLATE_##ENUM:                                                 \
        output += template.write.NAME(output, template.NAME, template.count);  \
        GSHL_FORMAT_DEBUG_PRINT(" result.string = \"%s\"\n", result.string);   \
        break;
                GHSL_TEMPLATE_KINDS
#undef KIND
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
                GSHL_UNUSED(templates);                                        \
                ADDITIONAL_TESTS                                               \
            } while (0);                                                       \
            GSHL_free_FormatWrapperResult(&result);                            \
        }

    TEST_FORMAT_WRAPPER_RESULT("", 0, {}, "");
    TEST_FORMAT_WRAPPER_RESULT("Hello world!\n", 13, {}, "Hello world!\n");
    TEST_FORMAT_WRAPPER_RESULT(
        "3245", 4,
        {
            GSHL_TEST_EQUAL(result.templates.count, 1);
            GSHL_TEST_EQUAL(templates[0].kind, GSHL_TEMPLATE_I32);
            GSHL_TEST_EQUAL(templates[0].i32, 3245);
            GSHL_TEST_EQUAL(templates[0].count, 4);
        },
        "%i", 3245);
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
        "%i %i %i", 1, -2, 3);

    TEST_FORMAT_WRAPPER_RESULT(
        "    -485   ", 11,
        {
            GSHL_TEST_EQUAL(result.templates.count, 1);
            GSHL_TEST_EQUAL(templates[0].kind, GSHL_TEMPLATE_I32);
            GSHL_TEST_EQUAL(templates[0].i32, -485);
            GSHL_TEST_EQUAL(templates[0].count, 4);
        },
        "    %i   ", -485);

    TEST_FORMAT_WRAPPER_RESULT(
        "Hello world!", 12,
        {
            GSHL_TEST_EQUAL(result.templates.count, 1);
            GSHL_TEST_EQUAL(templates[0].kind, GSHL_TEMPLATE_CSTRING);
            GSHL_TEST_STR_EQUAL(templates[0].cstring, "Hello world!");
            GSHL_TEST_EQUAL(templates[0].count, 12);
        },
        "%s", "Hello world!");

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
        "  %s  %s  %s ", "Foo", "bar", "baz");

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
        "%s = %i", "count", 5);

    TEST_FORMAT_WRAPPER_RESULT(
        "i32: 42", 7,
        {
            GSHL_TEST_EQUAL(result.templates.count, 1);
            GSHL_TEST_EQUAL(templates[0].kind, GSHL_TEMPLATE_I32);
            GSHL_TEST_EQUAL(templates[0].i32, 42);
            GSHL_TEST_EQUAL(templates[0].count, 2);
        },
        "i32: %i", 42)

    TEST_FORMAT_WRAPPER_RESULT(
        "u32: 42", 7,
        {
            GSHL_TEST_EQUAL(result.templates.count, 1);
            GSHL_TEST_EQUAL(templates[0].kind, GSHL_TEMPLATE_U32);
            GSHL_TEST_EQUAL(templates[0].u32, 42);
            GSHL_TEST_EQUAL(templates[0].count, 2);
        },
        "u32: %u", 42)

    TEST_FORMAT_WRAPPER_RESULT(
        "char: F", 7,
        {
            GSHL_TEST_EQUAL(result.templates.count, 1);
            GSHL_TEST_EQUAL(templates[0].kind, GSHL_TEMPLATE_CHAR);
            GSHL_TEST_EQUAL(templates[0].character, 'F');
            GSHL_TEST_EQUAL(templates[0].count, 1);
        },
        "char: %c", 'F')

    TEST_FORMAT_WRAPPER_RESULT(
        "void*: nil", 10,
        {
            GSHL_TEST_EQUAL(result.templates.count, 1);
            GSHL_TEST_EQUAL(templates[0].kind, GSHL_TEMPLATE_POINTER);
            GSHL_TEST_EQUAL(templates[0].pointer, NULL);
            GSHL_TEST_EQUAL(templates[0].count, 3);
        },
        "void*: %p", NULL)

    TEST_FORMAT_WRAPPER_RESULT(
        "void*: 0x7ffc0d1b64f4", 21,
        {
            GSHL_TEST_EQUAL(result.templates.count, 1);
            GSHL_TEST_EQUAL(templates[0].kind, GSHL_TEMPLATE_POINTER);
            GSHL_TEST_EQUAL(templates[0].pointer, (void *)0x7ffc0d1b64f4);
            GSHL_TEST_EQUAL(templates[0].count, 14);
        },
        "void*: %p", (void *)0x7ffc0d1b64f4)

    TEST_FORMAT_WRAPPER_RESULT(
        "%", 1,
        {
            GSHL_TEST_EQUAL(result.templates.count, 1);
            GSHL_TEST_EQUAL(templates[0].kind, GSHL_TEMPLATE_CHAR);
            GSHL_TEST_EQUAL(templates[0].character, '%');
            GSHL_TEST_EQUAL(templates[0].count, 1);
        },
        "%%")

    TEST_FORMAT_WRAPPER_RESULT(
        "0x69", 4,
        {
            GSHL_TEST_EQUAL(result.templates.count, 1);
            GSHL_TEST_EQUAL(templates[0].kind, GSHL_TEMPLATE_HEXADECIMAL);
            GSHL_TEST_EQUAL(templates[0].hex, 0x69);
            GSHL_TEST_EQUAL(templates[0].count, 4);
        },
        "%x", 105UL)

    TEST_FORMAT_WRAPPER_RESULT(
        "0x69 24", 7,
        {
            GSHL_TEST_EQUAL(result.templates.count, 2);
            GSHL_TEST_EQUAL(templates[0].kind, GSHL_TEMPLATE_HEXADECIMAL);
            GSHL_TEST_EQUAL(templates[0].hex, 105UL);
            GSHL_TEST_EQUAL(templates[0].count, 4);
            GSHL_TEST_EQUAL(templates[1].kind, GSHL_TEMPLATE_I32);
            GSHL_TEST_EQUAL(templates[1].i32, 24);
            GSHL_TEST_EQUAL(templates[1].count, 2);
        },
        "%x %i", 105, 24)

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
        "{{  }}")

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
        "{i8} {i16} {i32} {i64} {isize}", 127, 32767, -38453, 235638, 2349)

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
        "{u8}, {u16}, {u32}, {u64}, {usize}", 0xFF, 3422, 12893, 393294,
        34455677);

    // TEST_FORMAT_WRAPPER_RESULT("255, 3422, 12893, 393294, 34455677", 34, {},
    //                            " %i ", 0xFF, 3422, 12893, 393294, 34455677);

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

#    undef TEST_FORMAT_WRAPPER_RESULT
}

#    undef GSHL_FORMAT_DEBUG_FUNC
#    undef GSHL_FORMAT_DEBUG_PRINT

#endif // GSHL_TEST
