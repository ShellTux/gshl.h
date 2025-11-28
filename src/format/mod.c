#include "format/mod.h"

#include "format/bool.h"    // IWYU pragma: keep
#include "format/char.h"    // IWYU pragma: keep
#include "format/cstring.h" // IWYU pragma: keep
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

#include "ansi/mod.h"
#include "array/mod.h"
#include "hash_table/mod.h"
#include "macros/mod.h"
#include "print/mod.h"
#include "string/mod.h"

#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static __thread GSHL_HashTable GSHL_format_ht = {};

static usize GSHL_hash_format_specifier(const char *const start,
                                        const char *const end,
                                        const char **startP)
{
    u32 hash = 2166136261u; // FNV offset basis

    GSHL_ASSERT(start != NULL);

    if (startP == NULL) {
        GSHL_ASSERT(start <= end);

        // Reasonable length for a format specifier
        GSHL_ASSERT(end - start < GSHL_FORMAT_SPECIFIER_MAX_LEN &&
                    ("Exceeded max format specifier length"));

        for (const char *c = start; c < end; c += 1) {
            hash ^= *(unsigned char *)c;
            hash *= 16777619; // FNV prime
        }
    }
    else {
        GSHL_ASSERT(end == NULL);

        // Point to the first character of the format specifier
        const char *formatStart = *startP;
        // Point to one after the last character of the format specifier
        const char *formatEnd = *startP + 1;

        const char *specifierStart;
        const char *specifierEnd;

        switch (*formatStart) {
        case '%': {
            formatEnd = formatStart + (formatStart[1] == 'l') + 2;
            *startP = formatEnd;
            return GSHL_hash_format_specifier(formatStart, formatEnd, NULL);
        } break;
        case '{':
            specifierStart = formatStart + 1;
            specifierEnd = specifierStart;

            for (bool exit = false; !exit && *formatEnd != '\0';) {
                switch (*formatEnd) {
                case '}':
                case ';':
                    exit = true;
                    break;
                default:
                    formatEnd += 1;
                    break;
                }
            }

            specifierEnd = formatEnd;

            if (formatEnd[0] != '}') {
                fprintf(stderr, "Unknown format specifier \"%.*s\" \"%.*s\"\n",
                        (int)(formatEnd - formatStart), formatStart,
                        (int)(specifierEnd - specifierStart), specifierStart);
                return 0;
            }

            formatEnd += 1;
            *startP = formatEnd;

            return GSHL_hash_format_specifier(specifierStart, specifierEnd,
                                              NULL);
        default:
            return 0;
        }
    }

    return hash;
}

static usize GSHL_hash_idem(const GSHL_HashTableKey key) { return key.usize; }

__attribute__((constructor)) static void GSHL_init_ht()
{
    GSHL_FormatSpecifier format_specifiers[] = {
#define FS(BIT_SHIFT, ENUM, NAME, TYPE, OPTS, ...)                             \
    [BIT_SHIFT] = {                                                            \
        .kind = GSHL_FORMAT_SPECIFIER_##ENUM,                                  \
        .write = GSHL_write_##TYPE,                                            \
        .va_size = (((sizeof(TYPE) - 1) / 4) + 1) * 4,                         \
        .specifiers = {__VA_ARGS__},                                           \
    },
        GSHL_FORMAT_SPECIFIERS
#undef FS
    };

    GSHL_HashTable_init(&GSHL_format_ht, usize, GSHL_FormatSpecifier *,
                        GSHL_hash_idem);

    GSHL_ARRAY_FOREACH(format_specifiers, GSHL_FormatSpecifier fs)
    {
        if (fs.write == NULL) {
            continue;
        }

        GSHL_ASSERT(fs.va_size == 4 || fs.va_size == 8);

#if 0
        printf("FormatSpecifier {\n"
               "  .kind = %i\n"
               "  .va_size = %lu\n"
               "  .type_string = %s\n"
               "}\n",
               fs.kind, fs.va_size, fs.type_string);
#endif

        GSHL_format_specifier_register(fs);
    }

#if 0
    GSHL_HashTable_print(
        GSHL_format_ht, "%lu", usize, "%p", opaque,
        "FormatSpecifier {\n"
        "  .kind = %u\n"
        "  .write = %p\n"
        "  .va_size = %lu\n"
        "  .type_string = %s\n"
        "}",
        ((GSHL_FormatSpecifier *)current->value.opaque)->kind,
        ((GSHL_FormatSpecifier *)current->value.opaque)->write,
        ((GSHL_FormatSpecifier *)current->value.opaque)->va_size,
        ((GSHL_FormatSpecifier *)current->value.opaque)->type_string);
#endif
}

GSHL_StringView GSHL_format_wrapper(const char *restrict format, ...)
{
    va_list args;
    va_start(args, format);
    const GSHL_StringView view = GSHL_format_wrapperv(format, args);
    va_end(args);

    return view;
}

GSHL_StringView GSHL_format_wrapperv(const char *const restrict format,
                                     va_list args)
{
    GSHL_ASSERT(format != NULL);

    static __thread GSHL_FormatString strings[10] = {};
    static __thread usize strings_index = 0;

    GSHL_FormatString *string = &strings[strings_index];
    strings_index = (strings_index + 1) % GSHL_ARRAY_LEN(strings);

    GSHL_DArray_init(string);

    GSHL_format_writev(string, format, args);

    GSHL_DArray_append(string, '\0');

    return (GSHL_StringView){
        .start = string->items,
        .len = string->count - 1,
    };
}

usize GSHL_format_write(GSHL_FormatString *string,
                        const char *const restrict format, ...)
{
    va_list args;
    va_start(args, format);
    const usize count = GSHL_format_writev(string, format, args);
    va_end(args);
    return count;
}

usize GSHL_format_writev(GSHL_FormatString *string,
                         const char *const restrict format, va_list args)
{
    const char *formatEnd = format;

    for (const char *formatC = format; *formatC != '\0'; formatC += 1) {
        formatEnd = formatC;

        if ((formatC[0] == '{' && formatC[1] == '{') ||
            (formatC[0] == '}' && formatC[1] == '}')) {
            GSHL_DArray_append(string, formatC[1]);
            formatC += 1;
            continue;
        }

        const usize fs_hash =
            GSHL_hash_format_specifier(formatC, NULL, &formatEnd);

        if (formatC == formatEnd) {
            GSHL_DArray_append(string, *formatC);
        }
        else {
            const GSHL_HashTableValue *valueP =
                GSHL_HashTable_search(&GSHL_format_ht, .usize = fs_hash);
            if (valueP == NULL) {
                fprintf(stderr, "Unknown format specifier '%.*s'\n",
                        (int)(formatEnd - formatC), formatC);

                for (; formatC < formatEnd; formatC += 1) {
                    GSHL_DArray_append(string, *formatC);
                }

                formatC = formatEnd - 1;
                continue;
            }

            GSHL_FormatSpecifier *const fs = valueP->opaque;
            GSHL_ASSERT(fs != NULL);

            switch (fs->va_size) {
            case 4:
                fs->value.u32 = va_arg(args, u32);
                break;
            case 8:
                fs->value.u64 = va_arg(args, u64);
                break;
            default:
                GSHL_UNREACHABLE("Wrong fs->va_size = %lu", fs->va_size);
                break;
            }

            fs->write(string, fs);

            formatC = formatEnd - 1;
        }
    }

    GSHL_ASSERT(format <= formatEnd);

    return (usize)(formatEnd - format);
}

char *GSHL_format(const char *const restrict format, ...)
{
    va_list args;
    va_start(args, format);
    char *string = GSHL_formatv(format, args);
    va_end(args);

    return string;
}

char *GSHL_formatv(const char *const restrict format, va_list args)
{
    const GSHL_StringView view = GSHL_format_wrapperv(format, args);

    return GSHL_string_dup(view.start);
}

void GSHL_print_registered_format_specifiers(void)
{
#if 0
    GSHL_HashTable_print(
        GSHL_format_ht, "%lu", usize, "%p", opaque,
        "FormatSpecifier {\n"
        "    .kind = %u\n"
        "    .write = %p\n"
        "    .va_size = %lu\n"
        "    .type_string = %s\n"
        "  }",
        ((GSHL_FormatSpecifier *)current->value.opaque)->kind,
        ((GSHL_FormatSpecifier *)current->value.opaque)->write,
        ((GSHL_FormatSpecifier *)current->value.opaque)->va_size,
        ((GSHL_FormatSpecifier *)current->value.opaque)->type_string);
#else
    GSHL_ARRAYN_FOREACH(GSHL_format_ht.table, GSHL_format_ht.table_size,
                        GSHL_HashTableEntry * entry)
    {
        for (const GSHL_HashTableEntry *current = entry; current != NULL;
             current = current->next) {
            const GSHL_FormatSpecifier *fs = current->value.opaque;
            GSHL_ARRAY_FOREACH(fs->specifiers, char *specifier)
            {
                if (specifier == NULL || specifier[0] == '\0') {
                    break;
                }

                printf(GSHL_FG_CYAN("%s") "\n", specifier);
            }
        }
    }
#endif
}

bool GSHL_format_specifier_register(const GSHL_FormatSpecifier fs)
{
    static __thread GSHL_FormatSpecifiers format_specifiers = {};

    GSHL_DArray_append(&format_specifiers, fs);

    GSHL_ARRAY_FOREACH(fs.specifiers, char *specifier)
    {
        if (specifier == NULL || specifier[0] == '\0') {
            break;
        }

        const char *const start = specifier;
        const char *const end = specifier + strlen(specifier);
        const usize hash = GSHL_hash_format_specifier(start, end, NULL);
        if (GSHL_HashTable_search(&GSHL_format_ht, .usize = hash) != NULL) {
#ifdef GSHL_DEBUG
            GSHL_dprintln(STDERR_FILENO,
                          "Format Specifier already registered: %s", specifier);
#endif
            continue;
        }

        GSHL_ASSERT(fs.va_size % 4 == 0);

        GSHL_HashTable_insert(
            &GSHL_format_ht, .usize = hash,
            .opaque = &format_specifiers.items[format_specifiers.count - 1]);
    }

    return true;
}

#ifdef GSHL_TESTS
#    include "test/mod.h"

GSHL_TEST(format_wrapper)
{
#    define TEST_FORMAT_WRAPPER(EXPECTED_STRING, ...)                          \
        do {                                                                   \
            const char output[] = EXPECTED_STRING;                             \
            const usize expected_count = GSHL_STACK_STRING_LEN(output);        \
            const GSHL_StringView view = GSHL_format_wrapper(__VA_ARGS__);     \
            GSHL_TEST_EQUAL(view.len, expected_count);                         \
            GSHL_TEST_STR_EQUAL(view.start, output);                           \
        } while (0)

    TEST_FORMAT_WRAPPER("", "");
    TEST_FORMAT_WRAPPER("Hello world!", "Hello world!");
    TEST_FORMAT_WRAPPER("34 + 35 = 69", "%u + %u = %u", 34, 35, 69);
    TEST_FORMAT_WRAPPER("34 + 35 = 69", "{u32} + {u32} = {u32}", 34, 35, 69);

    TEST_FORMAT_WRAPPER("Foo bar baz", "%s %s %s", "Foo", "bar", "baz");
    TEST_FORMAT_WRAPPER("Foo bar baz", "{cstring} {cstring} {cstring}", "Foo",
                        "bar", "baz");

    // Test integer formatting
    TEST_FORMAT_WRAPPER("1 = 1", "%d = %d", 1, 1);
    TEST_FORMAT_WRAPPER("10 + 20 = 30", "%d + %d = %d", 10, 20, 30);
    TEST_FORMAT_WRAPPER("100 - 50 = 50", "%d - %d = %d", 100, 50, 50);
    TEST_FORMAT_WRAPPER("1 = 1", "{i32} = {i32}", 1, 1);
    TEST_FORMAT_WRAPPER("10 + 20 = 30", "{i32} + {i32} = {i32}", 10, 20, 30);
    TEST_FORMAT_WRAPPER("100 - 50 = 50", "{i32} - {i32} = {i32}", 100, 50, 50);

    // Test unsigned integer formatting
    TEST_FORMAT_WRAPPER("42 = 42", "%u = %u", 42, 42);
    TEST_FORMAT_WRAPPER("8 + 2 = 10", "%u + %u = %u", 8, 2, 10);
    TEST_FORMAT_WRAPPER("42 = 42", "{u32} = {u32}", 42, 42);
    TEST_FORMAT_WRAPPER("8 + 2 = 10", "{u32} + {u32} = {u32}", 8, 2, 10);

    // Test floating point formatting
    // TEST_FORMAT_WRAPPER("Pi ~ 3.14", "Pi is approximately %.2f", 3.14159);
    // TEST_FORMAT_WRAPPER("The number is 2.71828", "The number is
    // %f", 2.71828); TEST_FORMAT_WRAPPER("Gravity = 9.81 m/s^2", "Gravity =
    // %.2f m/s^2", 9.81);

    // Test character formatting
    TEST_FORMAT_WRAPPER("The first letter of the alphabet is A",
                        "The first letter of the alphabet is {character}", 'A');
    TEST_FORMAT_WRAPPER(
        "Coding",
        "{character}{character}{character}{character}{character}{character}",
        'C', 'o', 'd', 'i', 'n', 'g');

    // Test string formatting
    TEST_FORMAT_WRAPPER("Hello, World!", "%s", "Hello, World!");
    TEST_FORMAT_WRAPPER("Name: Alice", "Name: %s", "Alice");

    // Test hexadecimal formatting
    // TEST_FORMAT_WRAPPER("Hex 255 = FF", "Hex %d = %X", 255, 255);
    TEST_FORMAT_WRAPPER("Hex 16 = 10", "Hex %d = %x", 16, 16);

    // Test octal formatting
    // TEST_FORMAT_WRAPPER("Octal 8 = 10", "Octal %d = %o", 8, 8);
    // TEST_FORMAT_WRAPPER("Octal 64 = 100", "Octal %d = %o", 64, 64);

    // Test pointer formatting (simulated with address output)
    TEST_FORMAT_WRAPPER("Address of variable: 0x7ffef80057a4",
                        "Address of variable: %p", (void *)0x7ffef80057a4);

    // Test printing size_t
    usize size = 100;
    TEST_FORMAT_WRAPPER("Size = 100", "Size = %lu", size);

    TEST_FORMAT_WRAPPER("foo bar", "%s {string}", "foo", "bar");
    TEST_FORMAT_WRAPPER("34 35", "%u {u32}", 34, 35);
    TEST_FORMAT_WRAPPER("34 35 69", "%lu {u64} {usize}", 34L, 35L, 69L);
    TEST_FORMAT_WRAPPER("-34 -35 -69", "%i %d {i32}", -34, -35, -69);
    TEST_FORMAT_WRAPPER("-34 -35 -69", "%li %ld {isize}", -34L, -35L, -69L);
    TEST_FORMAT_WRAPPER("A B C", "%c {character} {char}", 'A', 'B', 'C');

#    undef TEST_FORMAT_WRAPPER
}

#    undef GSHL_FORMAT_SPECIFIER_ASSERT

#endif
