#include "format/isize.h"

#include "format/i16.h"
#include "format/i32.h"
#include "format/i64.h"
#include "format/i8.h"
#include "macros/mod.h"
#include "string/mod.h"

#include <assert.h>
#include <stdlib.h>

usize GSHL_write_isize(GSHL_FormatString *string,
                       const GSHL_FormatSpecifier *fs)
{
    GSHL_ASSERT(fs->kind &
                (GSHL_FORMAT_SPECIFIER_I8 | GSHL_FORMAT_SPECIFIER_I16 |
                 GSHL_FORMAT_SPECIFIER_I32 | GSHL_FORMAT_SPECIFIER_I64 |
                 GSHL_FORMAT_SPECIFIER_ISIZE));
    GSHL_ASSERT(fs->write == GSHL_write_i8 || fs->write == GSHL_write_i16 ||
                fs->write == GSHL_write_i32 || fs->write == GSHL_write_i64 ||
                fs->write == GSHL_write_isize);

    isize rem = 0;
    switch (fs->kind) {
    case GSHL_FORMAT_SPECIFIER_I8:
        rem = fs->value.i8;
        break;
    case GSHL_FORMAT_SPECIFIER_I16:
        rem = fs->value.i16;
        break;
    case GSHL_FORMAT_SPECIFIER_I32:
        rem = fs->value.i32;
        break;
    case GSHL_FORMAT_SPECIFIER_I64:
        rem = fs->value.i64;
        break;
    case GSHL_FORMAT_SPECIFIER_ISIZE:
        rem = fs->value.isize;
        break;
    default:
        GSHL_UNREACHABLE("Invalid kind %i", fs->kind);
        break;
    }

    bool neg = rem < 0;

    usize count = 0;

    if (neg) {
        rem *= -1;
        if (string != NULL) {
            GSHL_DArray_append(string, '-');
        }
        count += 1;
    }

    do {
        if (string != NULL) {
            GSHL_DArray_append(string, '0' + (rem % 10));
        }

        rem /= 10;
        count += 1;
    } while (rem > 0);

    if (string != NULL) {
        GSHL_string_reverse(&string->items[string->count - count + !!neg],
                            &string->items[string->count]);
    }

    return count;
}

#ifdef GSHL_TESTS
#    include "test/mod.h"

GSHL_TEST(write_isize)
{

#    define TEST_WRITE_isize(EXPRESSION, EXPECTED)                             \
        do {                                                                   \
            GSHL_FormatString string = {};                                     \
            GSHL_DArray_init(&string);                                         \
            const GSHL_FormatSpecifier fs = {                                  \
                .kind = GSHL_FORMAT_SPECIFIER_ISIZE,                           \
                .write = GSHL_write_isize,                                     \
                .value.isize = (EXPRESSION),                                   \
            };                                                                 \
                                                                               \
            const usize count = GSHL_write_isize(&string, &fs);                \
            GSHL_DArray_append(&string, '\0');                                 \
            GSHL_TEST_EQUAL(count, GSHL_STACK_STRING_LEN(EXPECTED));           \
            GSHL_TEST_STR_EQUAL(string.items, EXPECTED);                       \
            GSHL_DArray_destroy(&string);                                      \
        } while (0)

    TEST_WRITE_isize(42, "42");
    TEST_WRITE_isize(0, "0");
    TEST_WRITE_isize(294482, "294482");
    TEST_WRITE_isize(-294482, "-294482");
    TEST_WRITE_isize(~0, "-1");

#    undef TEST_WRITE_isize
}

#endif
