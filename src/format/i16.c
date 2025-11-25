#include "format/i16.h"

#include "format/isize.h"
#include "macros/mod.h"
#include "string/mod.h"

#include <assert.h>
#include <stdlib.h>

usize GSHL_write_i16(GSHL_FormatString *string, const GSHL_FormatSpecifier *fs)
{
    GSHL_ASSERT(fs->kind == GSHL_FORMAT_SPECIFIER_I16);
    GSHL_ASSERT(fs->write == GSHL_write_i16);

    return GSHL_write_isize(string, fs);
}

#ifdef GSHL_TESTS
#    include "test/mod.h"

GSHL_TEST(write_i16)
{

#    define TEST_WRITE_i16(EXPRESSION, EXPECTED)                               \
        do {                                                                   \
            GSHL_FormatString string = {};                                     \
            GSHL_DArray_init(&string);                                         \
            const GSHL_FormatSpecifier fs = {                                  \
                .kind = GSHL_FORMAT_SPECIFIER_I16,                             \
                .write = GSHL_write_i16,                                       \
                .value.i16 = (EXPRESSION),                                     \
            };                                                                 \
                                                                               \
            const usize count = GSHL_write_i16(&string, &fs);                  \
            GSHL_DArray_append(&string, '\0');                                 \
            GSHL_TEST_EQUAL(count, GSHL_STACK_STRING_LEN(EXPECTED));           \
            GSHL_TEST_STR_EQUAL(string.items, EXPECTED);                       \
            GSHL_DArray_destroy(&string);                                      \
        } while (0)

    TEST_WRITE_i16(42, "42");
    TEST_WRITE_i16(0, "0");
    TEST_WRITE_i16(2942, "2942");
    TEST_WRITE_i16(-2942, "-2942");
    TEST_WRITE_i16(~0, "-1");

#    undef TEST_WRITE_i16
}

#endif
