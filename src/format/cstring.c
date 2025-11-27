#include "format/cstring.h"

#include "macros/mod.h"
#include "string/mod.h"

#include <assert.h>
#include <stdlib.h>

usize GSHL_write_cstring(GSHL_FormatString *string,
                         const GSHL_FormatSpecifier *fs)
{
    GSHL_ASSERT(fs->kind == GSHL_FORMAT_SPECIFIER_CSTRING);
    GSHL_ASSERT(fs->write == GSHL_write_cstring);

    const usize start_count = string->count;

    GSHL_STRING_FOREACH(fs->value.cstring, c) { GSHL_DArray_append(string, c); }

    return string->count - start_count;
}

#ifdef GSHL_TESTS
#    include "test/mod.h"

GSHL_TEST(write_cstring)
{

#    define TEST_WRITE_cstring(EXPRESSION, EXPECTED)                           \
        do {                                                                   \
            GSHL_FormatString string = {};                                     \
            GSHL_DArray_init(&string);                                         \
            const GSHL_FormatSpecifier fs = {                                  \
                .kind = GSHL_FORMAT_SPECIFIER_CSTRING,                         \
                .write = GSHL_write_cstring,                                   \
                .value.cstring = (EXPRESSION),                                 \
            };                                                                 \
                                                                               \
            const usize count = GSHL_write_cstring(&string, &fs);              \
            GSHL_DArray_append(&string, '\0');                                 \
            GSHL_TEST_EQUAL(count, GSHL_STACK_STRING_LEN(EXPECTED));           \
            GSHL_TEST_STR_EQUAL(string.items, EXPECTED);                       \
            GSHL_DArray_destroy(&string);                                      \
        } while (0)

    TEST_WRITE_cstring("", "");
    TEST_WRITE_cstring("Hello world!", "Hello world!");
    TEST_WRITE_cstring("foo bar baz", "foo bar baz");

#    undef TEST_WRITE_cstring
}

#endif
