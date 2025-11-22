#include "format/cstring.h"
#include "macros/mod.h"
#include "string/mod.h"

#include <assert.h>
#include <string.h>

usize write_cstring(char *buf, GSHL_Template *t_mut)
{
    const GSHL_Template *const t = t_mut;

    GSHL_ASSERT(t->kind == GSHL_TEMPLATE_CSTRING);

    if (buf == NULL) {
        return t_mut->count = strlen(t->cstring);
    }

    memcpy(buf, t->cstring, t->count);

    return t->count;
}

#ifdef GSHL_TESTS
#    include "test/mod.h"

GSHL_TEST(write_cstring)
{
#    define TEST_WRITE_CSTRING(CSTRING, EXPECTED_STRING, ...)                  \
        {                                                                      \
            char buffer[256] = {0};                                            \
            struct GSHL_Template template = {                                  \
                .kind = GSHL_TEMPLATE_CSTRING,                                 \
                .cstring = CSTRING,                                            \
                .opts.cstring = {__VA_ARGS__},                                 \
            };                                                                 \
            const usize count = GSHL_STACK_STRING_LEN(EXPECTED_STRING);        \
            GSHL_TEST_EQUAL(write_cstring(NULL, &template), count);            \
            GSHL_TEST_EQUAL(write_cstring(buffer, &template), count);          \
            GSHL_TEST_STR_EQUAL(buffer, EXPECTED_STRING);                      \
        }

    TEST_WRITE_CSTRING("", "");
    TEST_WRITE_CSTRING("abc", "abc");
    TEST_WRITE_CSTRING("   4l45krjslkac  \n", "   4l45krjslkac  \n");

#    undef TEST_WRITE_CSTRING
}

#endif
