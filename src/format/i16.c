#include "format/i16.h"
#include "format/isize.h"

#include "macros/mod.h"
#include "string/mod.h"
#include "types/mod.h"

#include <assert.h>

usize write_i16(char *buf, GSHL_Template *t_mut)
{
    const GSHL_Template *const t = t_mut;

    GSHL_ASSERT(t->kind == GSHL_TEMPLATE_I16);

    return write_isize(buf, t_mut);
}

#ifdef GSHL_TESTS

#    include "test/mod.h"

GSHL_TEST(write_i16)
{
#    define TEST_WRITE_I16(NUMBER, EXPECTED_STRING, ...)                       \
        {                                                                      \
            char buffer[256] = {0};                                            \
            struct GSHL_Template template = {                                  \
                .kind = GSHL_TEMPLATE_I16,                                     \
                .i16 = NUMBER,                                                 \
                .opts.i16 = {__VA_ARGS__},                                     \
            };                                                                 \
            const usize count = GSHL_STACK_STRING_LEN(EXPECTED_STRING);        \
            GSHL_TEST_EQUAL(write_i16(NULL, &template), count);                \
            GSHL_TEST_EQUAL(write_i16(buffer, &template), count);              \
            GSHL_TEST_STR_EQUAL(buffer, EXPECTED_STRING);                      \
        }

    TEST_WRITE_I16(32767, "32767");
#    undef TEST_WRITE_isize
}

#endif
