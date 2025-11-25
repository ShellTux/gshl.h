#include "format/u16.h"

#include "format/usize.h"
#include "macros/mod.h"
#include "string/mod.h"

#include <assert.h>
#include <stdlib.h>

usize GSHL_write_u16(GSHL_FormatString *string, const GSHL_FormatSpecifier *fs)
{
    GSHL_ASSERT(fs->kind == GSHL_FORMAT_SPECIFIER_U16);
    GSHL_ASSERT(fs->write == GSHL_write_u16);

    return GSHL_write_usize(string, fs);
}

#ifdef GSHL_TESTS
#    include "test/mod.h"

GSHL_TEST(write_u16)
{

#    define TEST_WRITE_u16(EXPRESSION, EXPECTED)                               \
        do {                                                                   \
            GSHL_FormatString string = {};                                     \
            GSHL_DArray_init(&string);                                         \
            const GSHL_FormatSpecifier fs = {                                  \
                .kind = GSHL_FORMAT_SPECIFIER_U16,                             \
                .write = GSHL_write_u16,                                       \
                .value.u16 = (EXPRESSION),                                     \
            };                                                                 \
                                                                               \
            const u16 count = GSHL_write_u16(&string, &fs);                    \
            GSHL_DArray_append(&string, '\0');                                 \
            GSHL_TEST_EQUAL(count, GSHL_STACK_STRING_LEN(EXPECTED));           \
            GSHL_TEST_STR_EQUAL(string.items, EXPECTED);                       \
            GSHL_DArray_destroy(&string);                                      \
        } while (0)

    TEST_WRITE_u16(42, "42");
    TEST_WRITE_u16(0, "0");
    TEST_WRITE_u16(2944, "2944");
    TEST_WRITE_u16(0xFF, "255");

#    undef TEST_WRITE_u16
}

#endif
