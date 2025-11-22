#include "format/binary.h"

#include "macros/mod.h"
#include "string/mod.h"
#include "types/mod.h"

#include <assert.h>
#include <stdlib.h>

usize write_binary(char *buf, GSHL_Template *t_mut)
{
    const GSHL_Template *const t = t_mut;

    GSHL_ASSERT(t->kind == GSHL_TEMPLATE_BINARY);

    if (buf == NULL) {
        t_mut->count = 0;

        u64 remaining = t->binary;

        do {
            t_mut->count += 1;
            remaining >>= 1;
        } while (remaining != 0);

        return t->count;
    }

    usize count = 0;
    u64 remaining = t->binary;
    do {
        count += 1;

        buf[t->count - count] = "01"[remaining & 1];

        remaining >>= 1;
    } while (remaining != 0);

    GSHL_ASSERT(count == t->count);

    return count;
}

#ifdef GSHL_TESTS
#    include "test/mod.h"

GSHL_TEST(write_binary)
{
#    define TEST_WRITE_BINARY(NUMBER, EXPECTED_STRING, ...)                    \
        {                                                                      \
            char buffer[256] = {0};                                            \
            struct GSHL_Template template = {                                  \
                .kind = GSHL_TEMPLATE_BINARY,                                  \
                .binary = NUMBER,                                              \
                .opts.binary = {__VA_ARGS__},                                  \
            };                                                                 \
            const usize count = GSHL_STACK_STRING_LEN(EXPECTED_STRING);        \
            GSHL_TEST_EQUAL(write_binary(NULL, &template), count);             \
            GSHL_TEST_EQUAL(write_binary(buffer, &template), count);           \
            GSHL_TEST_STR_EQUAL(buffer, EXPECTED_STRING);                      \
        }

    TEST_WRITE_BINARY(0, "0");
    TEST_WRITE_BINARY(0b10101, "10101");
    TEST_WRITE_BINARY(1 << 5, "100000");
    TEST_WRITE_BINARY(
        ~0L,
        "1111111111111111111111111111111111111111111111111111111111111111");
    TEST_WRITE_BINARY(0xFF, "11111111");
#    undef TEST_WRITE_BINARY
}

#endif
