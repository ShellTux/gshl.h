#include "format/isize.h"

#include <assert.h>
#include <string.h>

usize write_isize(char *buf, const isize value, const usize precomputed_count)
{
    if (buf == NULL || precomputed_count == 0) {
        usize isize_count = 0;

        isize remaining = value;
        do {
            remaining /= 10;
            isize_count += 1;
        } while (remaining != 0);

        return (value >= 0 ? sizeof("") - 1 : sizeof("-") - 1) + isize_count;
    }

    if (value < 0) {
        buf[0] = '-';
    }
    else if (value == 0) {
        buf[0] = '0';
        return precomputed_count;
    }

    isize remaining = value;
    if (remaining < 0) {
        buf[0] = '-';
        remaining *= -1;
    }

    for (isize i = precomputed_count - 1; i >= 0 && remaining > 0;
         --i, remaining /= 10) {
        buf[i] = '0' + (remaining % 10);
    }

    return precomputed_count;
}

#ifdef GSHL_TESTS

#    include "test/mod.h"

GSHL_TEST(write_isize)
{
#    define TEST_WRITE_isize(NUMBER, EXPECTED_COUNT, EXPECTED_STRING)          \
        {                                                                      \
            char buffer[256] = {0};                                            \
            const usize count = write_isize(NULL, NUMBER, 0);                  \
            GSHL_TEST_EQUAL(count, EXPECTED_COUNT);                            \
            GSHL_TEST_EQUAL(write_isize(buffer, NUMBER, count),                \
                            EXPECTED_COUNT);                                   \
            GSHL_TEST_STR_EQUAL(buffer, EXPECTED_STRING);                      \
        }

    TEST_WRITE_isize(0, 1, "0");
    TEST_WRITE_isize(3245, 4, "3245");
    TEST_WRITE_isize(34948239, 8, "34948239");
    TEST_WRITE_isize(-0, 1, "0");
    TEST_WRITE_isize(-5, 2, "-5");
    TEST_WRITE_isize(-534, 4, "-534");
    // TEST_WRITE_isize(ISIZE_MAX, 10, "2147483647");
#    undef TEST_WRITE_isize
}

#endif
