#include "format/u8.h"

#include "format/usize.h"
#include "macros/mod.h"
#include "string/mod.h"

#include <assert.h>
#include <stdlib.h>

usize GSHL_write_u8(GSHL_FormatString *string, const GSHL_FormatSpecifier *fs)
{
    GSHL_ASSERT(fs->kind == GSHL_FORMAT_SPECIFIER_U8);
    GSHL_ASSERT(fs->write == GSHL_write_u8);

    return GSHL_write_usize(string, fs);
}

#ifdef GSHL_TESTS
#    include "test/mod.h"

GSHL_TEST(write_u8)
{

#    define TEST_WRITE_u8(EXPRESSION, EXPECTED)                                \
        do {                                                                   \
            GSHL_FormatString string = {};                                     \
            GSHL_DArray_init(&string);                                         \
            const GSHL_FormatSpecifier fs = {                                  \
                .kind = GSHL_FORMAT_SPECIFIER_U8,                              \
                .write = GSHL_write_u8,                                        \
                .value.u8 = (EXPRESSION),                                      \
            };                                                                 \
                                                                               \
            const u8 count = GSHL_write_u8(&string, &fs);                      \
            GSHL_DArray_append(&string, '\0');                                 \
            GSHL_TEST_EQUAL(count, GSHL_STACK_STRING_LEN(EXPECTED));           \
            GSHL_TEST_STR_EQUAL(string.items, EXPECTED);                       \
            GSHL_DArray_destroy(&string);                                      \
        } while (0)

    TEST_WRITE_u8(42, "42");
    TEST_WRITE_u8(0, "0");
    TEST_WRITE_u8(0xFF, "255");

#    undef TEST_WRITE_u8
}

#endif
