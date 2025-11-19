#include "format/u32.h"

#include "types/limits.h"
#include "types/mod.h"

#include <assert.h>
#include <string.h>

usize write_u32(char *buf, const u32 value, const usize precomputed_count)
{
    if (buf == NULL || precomputed_count == 0) {
        usize u32_count = 0;

        u32 remaining = value;
        do {
            remaining /= 10;
            u32_count += 1;
        } while (remaining != 0);

        return u32_count;
    }

    if (value == 0) {
        buf[0] = '0';
        return precomputed_count;
    }

    u32 remaining = value;
    for (isize i = precomputed_count - 1; i >= 0 && remaining > 0;
         --i, remaining /= 10) {
        buf[i] = '0' + (remaining % 10);
    }

    return precomputed_count;
}

#ifdef GSHL_TESTS

#    include "test/mod.h"

GSHL_TEST(write_u32)
{
#    define TEST_WRITE_U32(NUMBER, EXPECTED_COUNT, EXPECTED_STRING)            \
        {                                                                      \
            char buffer[256] = {0};                                            \
            const usize count = write_u32(NULL, NUMBER, 0);                    \
            GSHL_TEST_EQUAL(count, EXPECTED_COUNT);                            \
            GSHL_TEST_EQUAL(write_u32(buffer, NUMBER, count), EXPECTED_COUNT); \
            GSHL_TEST_STR_EQUAL(buffer, EXPECTED_STRING);                      \
        }

    TEST_WRITE_U32(0, 1, "0");
    TEST_WRITE_U32(3245, 4, "3245");
    TEST_WRITE_U32(34948239, 8, "34948239");
    TEST_WRITE_U32(U32_MAX, 10, "4294967295");
    TEST_WRITE_U32(U32_MIN, 1, "0");

#    undef TEST_WRITE_U32
}

#endif
