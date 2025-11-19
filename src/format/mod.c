#include "format/mod.h"
#include "format/char.h"
#include "format/cstring.h"
#include "format/hex.h"
#include "format/i32.h"

#include "array/mod.h"
#include "format/pointer.h"
#include "format/u32.h"
#include "macros/mod.h"
#include "types/mod.h"

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
