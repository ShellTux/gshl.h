#include "format/bool.h"

#include "dynamic_array/mod.h"
#include "macros/mod.h"
#include "string/mod.h"

#include <assert.h>
#include <stdlib.h>

usize GSHL_write_bool(GSHL_FormatString *string, const GSHL_FormatSpecifier *fs)
{
    GSHL_ASSERT(fs->kind == GSHL_FORMAT_SPECIFIER_BOOL);
    GSHL_ASSERT(fs->write == GSHL_write_bool);

    const bool boolean = !!fs->value.boolean;
    GSHL_ASSERT(boolean == 1 || boolean == 0);

    static const char conversions[][6] = {
        [false] = "false",
        [true] = "true",
    };

    usize count = 0;
    GSHL_STRING_FOREACH(conversions[boolean], c)
    {
        if (string != NULL) {
            GSHL_DArray_append(string, c);
        }

        count += 1;
    }

    return count;
}

#ifdef GSHL_TESTS
#    include "test/mod.h"

GSHL_TEST(write_bool)
{

#    define TEST_WRITE_bool(EXPRESSION, EXPECTED, ...)                         \
        do {                                                                   \
            GSHL_FormatString string = {};                                     \
            GSHL_DArray_init(&string);                                         \
            const GSHL_FormatSpecifier fs = {                                  \
                .kind = GSHL_FORMAT_SPECIFIER_BOOL,                            \
                .write = GSHL_write_bool,                                      \
                .value.boolean = (EXPRESSION),                                 \
                .opts.boolean = {__VA_ARGS__},                                 \
            };                                                                 \
                                                                               \
            const usize count = GSHL_write_bool(&string, &fs);                 \
            GSHL_DArray_append(&string, '\0');                                 \
            GSHL_TEST_EQUAL(count, GSHL_STACK_STRING_LEN(EXPECTED));           \
            GSHL_TEST_STR_EQUAL(string.items, EXPECTED);                       \
            GSHL_DArray_destroy(&string);                                      \
        } while (0)

    TEST_WRITE_bool(false, "false");
    TEST_WRITE_bool(true, "true");
    TEST_WRITE_bool(0, "false");
    TEST_WRITE_bool(1, "true");
    TEST_WRITE_bool(~0, "true");
    TEST_WRITE_bool(~1, "true");
    TEST_WRITE_bool(~~0, "false");
    TEST_WRITE_bool(!0, "true");
    TEST_WRITE_bool(!1, "false");

#    undef TEST_WRITE_bool
}

#endif
