#include "format/hex.h"
#include "format/pointer.h"

#include "array/mod.h"
#include "format/usize.h"
#include "macros/mod.h"
#include "string/mod.h"

#include <assert.h>
#include <stdlib.h>

usize GSHL_write_hex32(GSHL_FormatString *string,
                       const GSHL_FormatSpecifier *fs)
{
    GSHL_ASSERT(fs->kind == GSHL_FORMAT_SPECIFIER_HEX32);
    GSHL_ASSERT(fs->write == GSHL_write_hex32);

    GSHL_FormatSpecifier fs_copy = *fs;
    fs_copy.value.hex64 = fs->value.hex32;

    return GSHL_write_hex64(string, &fs_copy);
}

usize GSHL_write_hex64(GSHL_FormatString *string,
                       const GSHL_FormatSpecifier *fs)
{
    GSHL_ASSERT(fs->kind &
                (GSHL_FORMAT_SPECIFIER_HEX32 | GSHL_FORMAT_SPECIFIER_HEX64 |
                 GSHL_FORMAT_SPECIFIER_POINTER));
    GSHL_ASSERT(fs->write == GSHL_write_hex32 ||
                fs->write == GSHL_write_hex64 ||
                fs->write == GSHL_write_pointer);

    static const char conversion[][17] = {
        [false] = "0123456789abcdef",
        [true] = "0123456789ABCDEF",
    };

    const char *convert =
        conversion[fs->opts.hex64.uppercase % GSHL_ARRAY_LEN(conversion)];

    usize count = 0;
    hex64 rem = fs->value.hex64;
    do {
        if (string != NULL) {
            GSHL_DArray_append(string, convert[rem % 16]);
        }

        rem >>= 4;
        count += 1;
    } while (rem != 0);

    if (string != NULL) {
        GSHL_string_reverse(&string->items[string->count - count],
                            &string->items[string->count]);
    }

    return count;
}

#ifdef GSHL_TESTS
#    include "test/mod.h"

GSHL_TEST(write_hex32)
{

#    define TEST_WRITE_hex32(EXPRESSION, EXPECTED, ...)                        \
        do {                                                                   \
            GSHL_FormatString string = {};                                     \
            GSHL_DArray_init(&string);                                         \
            const GSHL_FormatSpecifier fs = {                                  \
                .kind = GSHL_FORMAT_SPECIFIER_HEX32,                           \
                .write = GSHL_write_hex32,                                     \
                .value.hex32 = (EXPRESSION),                                   \
                .opts.hex32 = {__VA_ARGS__},                                   \
            };                                                                 \
                                                                               \
            const hex32 count = GSHL_write_hex32(&string, &fs);                \
            GSHL_DArray_append(&string, '\0');                                 \
            GSHL_TEST_EQUAL(count, GSHL_STACK_STRING_LEN(EXPECTED));           \
            GSHL_TEST_STR_EQUAL(string.items, EXPECTED);                       \
            GSHL_DArray_destroy(&string);                                      \
        } while (0)

#    define TEST_WRITE_hex64(EXPRESSION, EXPECTED, ...)                        \
        do {                                                                   \
            GSHL_FormatString string = {};                                     \
            GSHL_DArray_init(&string);                                         \
            const GSHL_FormatSpecifier fs = {                                  \
                .kind = GSHL_FORMAT_SPECIFIER_HEX64,                           \
                .write = GSHL_write_hex64,                                     \
                .value.hex64 = (EXPRESSION),                                   \
                .opts.hex64 = {__VA_ARGS__},                                   \
            };                                                                 \
                                                                               \
            const hex64 count = GSHL_write_hex64(&string, &fs);                \
            GSHL_DArray_append(&string, '\0');                                 \
            GSHL_TEST_EQUAL(count, GSHL_STACK_STRING_LEN(EXPECTED));           \
            GSHL_TEST_STR_EQUAL(string.items, EXPECTED);                       \
            GSHL_DArray_destroy(&string);                                      \
        } while (0)

    TEST_WRITE_hex32(0xFF, "ff");
    TEST_WRITE_hex32(0xFF, "FF", .uppercase = true);
    TEST_WRITE_hex64(0xFF, "ff");
    TEST_WRITE_hex64(0xFF, "FF", .uppercase = true);

#    undef TEST_WRITE_hex32
}

#endif
