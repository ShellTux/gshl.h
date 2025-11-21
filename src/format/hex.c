#include "format/hex.h"

#include <string.h>

usize write_hex(char *buf, const usize hex, const usize precomputed_count)
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

    usize remaining = hex;
    for (isize i = precomputed_count - 1; i >= 0; --i) {
        buf[i] = "0123456789abcdef"[remaining % 16];
        remaining >>= 4;
    }

    return precomputed_count;
}

#ifdef GSHL_TESTS

#    include "test/mod.h"

GSHL_TEST(write_hex)
{
#    define TEST_WRITE_HEX(NUMBER, EXPECTED_COUNT, EXPECTED_STRING)            \
        {                                                                      \
            char buffer[256] = {0};                                            \
            const usize count = write_hex(NULL, NUMBER, 0);                    \
            GSHL_TEST_EQUAL(count, EXPECTED_COUNT);                            \
            GSHL_TEST_EQUAL(write_hex(buffer, NUMBER, count), EXPECTED_COUNT); \
            GSHL_TEST_STR_EQUAL(buffer, EXPECTED_STRING);                      \
        }

    TEST_WRITE_HEX(0, 1, "0");
    TEST_WRITE_HEX(0xff, 2, "ff");

#    undef TEST_WRITE_HEX
}

#endif
