#include "format/hex.h"

#include <string.h>

usize write_hex(char *buf, const usize hex,
                const struct GSHL_TemplateOpts_hex opts,
                const usize precomputed_count)
{
    if (buf == NULL || precomputed_count == 0) {
        usize count = 0;
        usize remaining = hex;
        do {
            remaining >>= 4;
            count += 1;
        } while (remaining != 0);

        return count;
    }

    const char conversion[2][17] = {
        [false] = "0123456789abcdef",
        [true] = "0123456789ABCDEF",
    };

    usize remaining = hex;
    for (isize i = precomputed_count - 1; i >= 0; --i) {
        buf[i] = conversion[opts.uppercase][remaining % 16];
        remaining >>= 4;
    }

    return precomputed_count;
}

#ifdef GSHL_TESTS

#    include "test/mod.h"

GSHL_TEST(write_hex)
{
#    define TEST_WRITE_HEX(NUMBER, EXPECTED_STRING, ...)                       \
        {                                                                      \
            char buffer[256] = {0};                                            \
            const struct GSHL_TemplateOpts_hex optsDef = {__VA_ARGS__};        \
            const usize count = write_hex(NULL, NUMBER, optsDef, 0);           \
            GSHL_TEST_EQUAL(count, sizeof(EXPECTED_STRING) - 1);               \
            GSHL_TEST_EQUAL(write_hex(buffer, NUMBER, optsDef, count),         \
                            sizeof(EXPECTED_STRING) - 1);                      \
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
