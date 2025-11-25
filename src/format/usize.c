#include "format/usize.h"

#include "format/u16.h"
#include "format/u32.h"
#include "format/u64.h"
#include "format/u8.h"
#include "macros/mod.h"
#include "string/mod.h"

#include <assert.h>
#include <stdlib.h>

usize GSHL_write_usize(GSHL_FormatString *string,
                       const GSHL_FormatSpecifier *fs)
{
    GSHL_ASSERT(fs->kind &
                (GSHL_FORMAT_SPECIFIER_U8 | GSHL_FORMAT_SPECIFIER_U16 |
                 GSHL_FORMAT_SPECIFIER_U32 | GSHL_FORMAT_SPECIFIER_U64 |
                 GSHL_FORMAT_SPECIFIER_USIZE));
    GSHL_ASSERT(fs->write == GSHL_write_u8 || fs->write == GSHL_write_u16 ||
                fs->write == GSHL_write_u32 || fs->write == GSHL_write_u64 ||
                fs->write == GSHL_write_usize);

    usize rem = 0;
    switch (fs->kind) {
    case GSHL_FORMAT_SPECIFIER_U8:
        rem = fs->value.u8;
        break;
    case GSHL_FORMAT_SPECIFIER_U16:
        rem = fs->value.u16;
        break;
    case GSHL_FORMAT_SPECIFIER_U32:
        rem = fs->value.u32;
        break;
    case GSHL_FORMAT_SPECIFIER_U64:
        rem = fs->value.u64;
        break;
    case GSHL_FORMAT_SPECIFIER_USIZE:
        rem = fs->value.usize;
        break;
    default:
        GSHL_UNREACHABLE("Invalid kind %i", fs->kind);
        break;
    }

    usize count = 0;
    do {
        if (string != NULL) {
            GSHL_DArray_append(string, '0' + (rem % 10));
        }

        rem /= 10;
        count += 1;
    } while (rem > 0);

    if (string != NULL) {
        GSHL_string_reverse(&string->items[string->count - count],
                            &string->items[string->count]);
    }

    return count;
}

#ifdef GSHL_TESTS
#    include "test/mod.h"

GSHL_TEST(write_usize)
{

#    define TEST_WRITE_usize(EXPRESSION, EXPECTED)                             \
        do {                                                                   \
            GSHL_FormatString string = {};                                     \
            GSHL_DArray_init(&string);                                         \
            const GSHL_FormatSpecifier fs = {                                  \
                .kind = GSHL_FORMAT_SPECIFIER_USIZE,                           \
                .write = GSHL_write_usize,                                     \
                .value.usize = (EXPRESSION),                                   \
            };                                                                 \
                                                                               \
            const usize count = GSHL_write_usize(&string, &fs);                \
            GSHL_DArray_append(&string, '\0');                                 \
            GSHL_TEST_EQUAL(count, GSHL_STACK_STRING_LEN(EXPECTED));           \
            GSHL_TEST_STR_EQUAL(string.items, EXPECTED);                       \
            GSHL_DArray_destroy(&string);                                      \
        } while (0)

    TEST_WRITE_usize(42, "42");
    TEST_WRITE_usize(0, "0");
    TEST_WRITE_usize(294482, "294482");
    TEST_WRITE_usize(-294482U, "4294672814");
    TEST_WRITE_usize(~0U, "4294967295");
    TEST_WRITE_usize(0xFF, "255");

#    undef TEST_WRITE_usize
}

#endif
