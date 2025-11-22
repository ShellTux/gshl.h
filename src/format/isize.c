#include "format/isize.h"

#include "macros/mod.h"
#include "string/mod.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

usize write_isize(char *buf, GSHL_Template *t_mut)
{
    const GSHL_Template *const t = t_mut;

    GSHL_ASSERT(t->kind &
                (GSHL_TEMPLATE_I8 | GSHL_TEMPLATE_I16 | GSHL_TEMPLATE_I32 |
                 GSHL_TEMPLATE_I64 | GSHL_TEMPLATE_ISIZE));

    isize remaining = t->i32 * !!(t->kind & GSHL_TEMPLATE_I8) +
                      t->i32 * !!(t->kind & GSHL_TEMPLATE_I16) +
                      t->i32 * !!(t->kind & GSHL_TEMPLATE_I32) +
                      t->i64 * !!(t->kind & GSHL_TEMPLATE_I64) +
                      t->isize * !!(t->kind & GSHL_TEMPLATE_ISIZE);

    if (buf == NULL) {
        if (remaining < 0) {
            t_mut->count = GSHL_STACK_STRING_LEN("-");
            remaining *= -1;
        }
        else {
            t_mut->count = 0;
        }

        do {
            remaining /= 10;
            t_mut->count += 1;
        } while (remaining != 0);

        return t->count;
    }

    if (remaining == 0) {
        GSHL_ASSERT(t->count == 1);
        buf[0] = '0';
        return t->count;
    }

    if (remaining < 0) {
        buf[0] = '-';
        remaining *= -1;
    }

    for (isize i = t->count - 1; i >= 0 && remaining > 0;
         --i, remaining /= 10) {
        buf[i] = '0' + (remaining % 10);
    }

    return t->count;
}

#ifdef GSHL_TESTS

#    include "test/mod.h"

GSHL_TEST(write_isize)
{
#    define TEST_WRITE_ISIZE(NUMBER, EXPECTED_STRING, ...)                     \
        {                                                                      \
            char buffer[256] = {0};                                            \
            struct GSHL_Template template = {                                  \
                .kind = GSHL_TEMPLATE_ISIZE,                                   \
                .isize = NUMBER,                                               \
                .opts.isize = {__VA_ARGS__},                                   \
            };                                                                 \
            const usize count = GSHL_STACK_STRING_LEN(EXPECTED_STRING);        \
            GSHL_TEST_EQUAL(write_isize(NULL, &template), count);              \
            GSHL_TEST_EQUAL(write_isize(buffer, &template), count);            \
            GSHL_TEST_STR_EQUAL(buffer, EXPECTED_STRING);                      \
        }

    TEST_WRITE_ISIZE(0, "0");
    TEST_WRITE_ISIZE(3245, "3245");
    TEST_WRITE_ISIZE(34948239, "34948239");
    TEST_WRITE_ISIZE(-0, "0");
    TEST_WRITE_ISIZE(-2, "-2");
    TEST_WRITE_ISIZE(-5, "-5");
    TEST_WRITE_ISIZE(-534, "-534");
    TEST_WRITE_ISIZE(-69, "-69");
    // TEST_WRITE_isize(ISIZE_MAX, 10, "2147483647");
#    undef TEST_WRITE_isize
}

#endif
