#include "format/char.h"

#include "macros/mod.h"
#include "string/mod.h"

#include <assert.h>
#include <stdlib.h>

usize GSHL_write_char(GSHL_FormatString *string, const GSHL_FormatSpecifier *fs)
{
    GSHL_ASSERT(fs->kind == GSHL_FORMAT_SPECIFIER_CHAR);
    GSHL_ASSERT(fs->write == GSHL_write_char);

    const char c = fs->value.character;
    if (string != NULL) {
        GSHL_DArray_append(string, c);
    }

    return 1;
}

#ifdef GSHL_TESTS
#    include "test/mod.h"

GSHL_TEST(write_char)
{

#    define TEST_WRITE_char(EXPRESSION, EXPECTED)                              \
        do {                                                                   \
            GSHL_FormatString string = {};                                     \
            GSHL_DArray_init(&string);                                         \
            const GSHL_FormatSpecifier fs = {                                  \
                .kind = GSHL_FORMAT_SPECIFIER_CHAR,                            \
                .write = GSHL_write_char,                                      \
                .value.character = (EXPRESSION),                               \
            };                                                                 \
                                                                               \
            const usize count = GSHL_write_char(&string, &fs);                 \
            const char expected[] = EXPECTED;                                  \
            GSHL_DArray_append(&string, '\0');                                 \
            GSHL_TEST_EQUAL(count, 1);                                         \
            GSHL_TEST_STR_EQUAL(string.items, expected);                       \
            GSHL_DArray_destroy(&string);                                      \
        } while (0)

    TEST_WRITE_char(0, "");
    TEST_WRITE_char('0', "0");
    TEST_WRITE_char('1', "1");
    TEST_WRITE_char('2', "2");
    TEST_WRITE_char('3', "3");
    TEST_WRITE_char('4', "4");
    TEST_WRITE_char('%', "%");
    TEST_WRITE_char('\0', "");

#    undef TEST_WRITE_char
}

#endif
