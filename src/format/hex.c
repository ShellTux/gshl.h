#include "format/hex.h"
#include "macros/mod.h"
#include "string/mod.h"

#include <assert.h>
#include <string.h>

usize write_hex(char *buf, GSHL_Template *t_mut)
{
    const GSHL_Template *const t = t_mut;

    GSHL_ASSERT(t->kind == GSHL_TEMPLATE_HEXADECIMAL);

    if (buf == NULL) {
        t_mut->count = 0;

        usize remaining = t->hex;
        do {
            remaining >>= 4;
            t_mut->count += 1;
        } while (remaining != 0);

        return t->count;
    };

    static const char conversion[][17] = {
        [false] = "0123456789abcdef",
        [true] = "0123456789ABCDEF",
    };

    usize remaining = t->hex;
    usize count = 0;
    for (isize i = t->count - 1; i >= 0; --i) {
        buf[i] = conversion[t->opts.hex.uppercase][remaining % 16];
        remaining >>= 4;
        count += 1;
    }

    GSHL_ASSERT(count == t->count);

    return t->count;
}

#ifdef GSHL_TESTS

#    include "test/mod.h"

GSHL_TEST(write_hex)
{
#    define TEST_WRITE_HEX(NUMBER, EXPECTED_STRING, ...)                       \
        {                                                                      \
            char buffer[256] = {0};                                            \
            struct GSHL_Template template = {                                  \
                .kind = GSHL_TEMPLATE_HEXADECIMAL,                             \
                .hex = NUMBER,                                                 \
                .opts.hex = {__VA_ARGS__},                                     \
            };                                                                 \
            const usize count = GSHL_STACK_STRING_LEN(EXPECTED_STRING);        \
            GSHL_TEST_EQUAL(write_hex(NULL, &template), count);                \
            GSHL_TEST_EQUAL(write_hex(buffer, &template), count);              \
            GSHL_TEST_STR_EQUAL(buffer, EXPECTED_STRING);                      \
        }

    TEST_WRITE_HEX(0, "0");
    TEST_WRITE_HEX(0x0123456789abcde, "123456789abcde");
    TEST_WRITE_HEX(0x0123456789abcde, "123456789ABCDE", .uppercase = true);
    TEST_WRITE_HEX(0xabcdef0123456789, "abcdef0123456789", .uppercase = false);
    TEST_WRITE_HEX(0XABCDEF0123456789, "ABCDEF0123456789", .uppercase = true);

#    undef TEST_WRITE_HEX
}

#endif
