#include "format/binary.h"

#include "macros/mod.h"
#include "types/mod.h"

#include <stdlib.h>

usize write_binary(char *buf, const u64 binary, const usize precomputed_count)
{
    if (buf == NULL || precomputed_count == 0) {
        usize count = 0;
        u64 remaining = binary;

        do {
            count += 1;
            remaining >>= 1;
        } while (remaining != 0);

        return count;
    }

    usize count = 0;
    u64 remaining = binary;
    do {
        count += 1;

        buf[precomputed_count - count] = "01"[remaining & 1];

        remaining >>= 1;
    } while (remaining != 0);

    GSHL_ASSERT(count == precomputed_count);

    return precomputed_count;
}

#ifdef GSHL_TESTS
#    include "test/mod.h"

GSHL_TEST(write_binary)
{
#    define TEST_WRITE_BINARY(NUMBER, EXPECTED_COUNT, EXPECTED_STRING)         \
        {                                                                      \
            char buffer[256] = {0};                                            \
            const usize count = write_binary(NULL, NUMBER, 0);                 \
            GSHL_TEST_EQUAL(count, EXPECTED_COUNT);                            \
            GSHL_TEST_EQUAL(write_binary(buffer, NUMBER, count),               \
                            EXPECTED_COUNT);                                   \
            GSHL_TEST_STR_EQUAL(buffer, EXPECTED_STRING);                      \
        }

    TEST_WRITE_BINARY(0, 1, "0");
    TEST_WRITE_BINARY(0b10101, 5, "10101");
    TEST_WRITE_BINARY(1 << 5, 6, "100000");
    TEST_WRITE_BINARY(
        ~0L, 64,
        "1111111111111111111111111111111111111111111111111111111111111111");
    TEST_WRITE_BINARY(0xFF, 8, "11111111");
#    undef TEST_WRITE_BINARY
}

#endif
