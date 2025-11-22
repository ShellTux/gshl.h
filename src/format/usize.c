#include "format/usize.h"

#include "macros/mod.h"
#include "string/mod.h"
#include "types/mod.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

usize write_usize(char *buf, GSHL_Template *t_mut)
{
    const GSHL_Template *const t = t_mut;

    GSHL_ASSERT(t->kind &
                (GSHL_TEMPLATE_U8 | GSHL_TEMPLATE_U16 | GSHL_TEMPLATE_U32 |
                 GSHL_TEMPLATE_U64 | GSHL_TEMPLATE_USIZE));

    usize remaining = t->u8 * !!(t->kind & GSHL_TEMPLATE_U8) +
                      t->u16 * !!(t->kind & GSHL_TEMPLATE_U16) +
                      t->u32 * !!(t->kind & GSHL_TEMPLATE_U32) +
                      t->u64 * !!(t->kind & GSHL_TEMPLATE_U64) +
                      t->usize * !!(t->kind & GSHL_TEMPLATE_USIZE);

    if (buf == NULL) {
        t_mut->count = 0;

        do {
            remaining /= 10;
            t_mut->count += 1;
        } while (remaining != 0);

        return t->count;
    }

    if (remaining == 0) {
        buf[0] = '0';
        return t->count;
    }

    for (isize i = t->count - 1; i >= 0 && remaining > 0;
         --i, remaining /= 10) {
        buf[i] = '0' + (remaining % 10);
    }

    GSHL_ASSERT(remaining == 0);

    return t->count;
}

#ifdef GSHL_TESTS

#    include "test/mod.h"

GSHL_TEST(write_usize)
{
#    define TEST_WRITE_USIZE(NUMBER, EXPECTED_STRING, ...)                     \
        {                                                                      \
            char buffer[256] = {0};                                            \
            struct GSHL_Template template = {                                  \
                .kind = GSHL_TEMPLATE_USIZE,                                   \
                .usize = NUMBER,                                               \
                .opts.usize = {__VA_ARGS__},                                   \
            };                                                                 \
            const usize count = GSHL_STACK_STRING_LEN(EXPECTED_STRING);        \
            GSHL_TEST_EQUAL(write_usize(NULL, &template), count);              \
            GSHL_TEST_EQUAL(write_usize(buffer, &template), count);            \
            GSHL_TEST_STR_EQUAL(buffer, EXPECTED_STRING);                      \
        }

    {
        char buffer[256] = {0};
        struct GSHL_Template template = {
            .kind = GSHL_TEMPLATE_USIZE,
            .usize = 0,
            .opts.usize = {},
        };
        const usize count = GSHL_STACK_STRING_LEN("0");
        GSHL_TEST_EQUAL(write_usize(NULL, &template), count);
        GSHL_TEST_EQUAL(write_usize(buffer, &template), count);
        GSHL_TEST_STR_EQUAL(buffer, "0");
    }
    TEST_WRITE_USIZE(0, "0");
    TEST_WRITE_USIZE(3245, "3245");
    TEST_WRITE_USIZE(34948239, "34948239");
    // TEST_WRITE_usize(usize_MAX, 10, "4294967295");
    // TEST_WRITE_usize(usize_MIN, 1, "0");

#    undef TEST_WRITE_usize
}

#endif
