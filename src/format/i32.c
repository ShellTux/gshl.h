#include "format/i32.h"
#include "types/limits.h"
#include "types/mod.h"

#include <assert.h>
#include <string.h>

usize write_i32(char *buf, const i32 value, const usize precomputed_count)
{
    if (buf == NULL || precomputed_count == 0) {
        usize i32_count = 0;

        i32 remaining = value;
        do {
            remaining /= 10;
            i32_count += 1;
        } while (remaining != 0);

        return (value >= 0 ? sizeof("") - 1 : sizeof("-") - 1) + i32_count;
    }

    const usize count = precomputed_count;
    if (value < 0) {
        buf[0] = '-';
    }
    else if (value == 0) {
        buf[0] = '0';
        return count;
    }

    i32 remaining = value;
    if (remaining < 0) {
        buf[0] = '-';
        remaining *= -1;
    }

    for (isize i = count - 1; i >= 0 && remaining > 0; --i, remaining /= 10) {
        buf[i] = '0' + (remaining % 10);
    }

    return count;
}

#ifdef GSHL_TESTS

#    include "test/mod.h"

GSHL_TEST(write_i32)
{
    GSHL_TEST_EQUAL(write_i32(NULL, 0, 0), 1);
    GSHL_TEST_EQUAL(write_i32(NULL, 34948239, 0), 8);
    GSHL_TEST_EQUAL(write_i32(NULL, -0, 0), 1);
    GSHL_TEST_EQUAL(write_i32(NULL, -5, 0), 2);
    GSHL_TEST_EQUAL(write_i32(NULL, -534, 0), 4);
    GSHL_TEST_EQUAL(write_i32(NULL, I32_MAX, 0), 10);
    GSHL_TEST_EQUAL(write_i32(NULL, I32_MIN, 0), 11);

#    define TEST_WRITE_I32(NUMBER, EXPECTED_COUNT, EXPECTED_STRING)            \
        {                                                                      \
            char buffer[256] = {0};                                            \
            const usize count = write_i32(NULL, NUMBER, 0);                    \
            GSHL_TEST_EQUAL(count, EXPECTED_COUNT);                            \
            GSHL_TEST_EQUAL(write_i32(buffer, NUMBER, count), EXPECTED_COUNT); \
            GSHL_TEST_STR_EQUAL(buffer, EXPECTED_STRING);                      \
        }

    TEST_WRITE_I32(0, 1, "0");
    TEST_WRITE_I32(3245, 4, "3245");
    TEST_WRITE_I32(34948239, 8, "34948239");
    TEST_WRITE_I32(-0, 1, "0");
    TEST_WRITE_I32(-5, 2, "-5");
    TEST_WRITE_I32(-534, 4, "-534");
    TEST_WRITE_I32(I32_MAX, 10, "2147483647");
    {
        // GSHL_TEST_EQUAL(write_i32(buffer, I32_MIN), 11);
        // GSHL_TEST_STR_EQUAL(buffer, "");
    }
#    undef TEST_WRITE_I32
}

#endif
