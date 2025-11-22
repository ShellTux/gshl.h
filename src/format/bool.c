#include "format/bool.h"

#include "macros/mod.h"
#include "string/mod.h"
#include "types/mod.h"

#include <assert.h>
#include <string.h>

usize write_boolean(char *buf, GSHL_Template *t_mut)
{
    const GSHL_Template *const t = t_mut;

    GSHL_ASSERT(t->kind == GSHL_TEMPLATE_BOOL);

    static const char *strings[] = {
        [false] = "false",
        [true] = "true",
    };

    static const usize lens[] = {
        [false] = GSHL_STACK_STRING_LEN("false"),
        [true] = GSHL_STACK_STRING_LEN("true"),
    };

    if (buf == NULL) {
        return t_mut->count = lens[t->boolean];
    }

    GSHL_ASSERT(t->count == lens[t->boolean]);

    memcpy(buf, strings[t->boolean], t->count);

    return t->count;
}

#ifdef GSHL_TESTS
#    include "test/mod.h"

GSHL_TEST(write_boolean)
{
#    define TEST_WRITE_BOOLEAN(NUMBER, EXPECTED_STRING, ...)                   \
        {                                                                      \
            char buffer[256] = {0};                                            \
            struct GSHL_Template template = {                                  \
                .kind = GSHL_TEMPLATE_BOOL,                                    \
                .boolean = NUMBER,                                             \
                .opts.boolean = {__VA_ARGS__},                                 \
            };                                                                 \
            const usize count = GSHL_STACK_STRING_LEN(EXPECTED_STRING);        \
            GSHL_TEST_EQUAL(write_boolean(NULL, &template), count);            \
            GSHL_TEST_EQUAL(write_boolean(buffer, &template), count);          \
            GSHL_TEST_STR_EQUAL(buffer, EXPECTED_STRING);                      \
        }

    TEST_WRITE_BOOLEAN(0, "false");
    TEST_WRITE_BOOLEAN(1, "true");
    TEST_WRITE_BOOLEAN(0b10101, "true");
    TEST_WRITE_BOOLEAN(~0L, "true");
    TEST_WRITE_BOOLEAN(true, "true");
    TEST_WRITE_BOOLEAN(false, "false");

#    undef TEST_WRITE_BOOLEAN
}

#endif
