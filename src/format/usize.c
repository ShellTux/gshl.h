#include "format/usize.h"

#include "types/mod.h"

#include <assert.h>
#include <string.h>

usize write_usize(char *buf, const usize value,
                  const struct GSHL_TemplateOpts_usize opts,
                  const usize precomputed_count)
{
    if (buf == NULL || precomputed_count == 0) {
        usize usize_count = 0;

        usize remaining = value;
        do {
            remaining /= 10;
            usize_count += 1;
        } while (remaining != 0);

        return usize_count;
    }

    if (value == 0) {
        buf[0] = '0';
        return precomputed_count;
    }

    usize remaining = value;
    for (isize i = precomputed_count - 1; i >= 0 && remaining > 0;
         --i, remaining /= 10) {
        buf[i] = '0' + (remaining % 10);
    }

    return precomputed_count;
}

#ifdef GSHL_TESTS

#    include "test/mod.h"

GSHL_TEST(write_usize)
{
#    define TEST_WRITE_usize(NUMBER, EXPECTED_COUNT, EXPECTED_STRING, ...)     \
        {                                                                      \
            char buffer[256] = {0};                                            \
            const struct GSHL_TemplateOpts_usize optsDef = {__VA_ARGS__};      \
            const usize count = write_usize(NULL, NUMBER, optsDef, 0);         \
            GSHL_TEST_EQUAL(count, EXPECTED_COUNT);                            \
            GSHL_TEST_EQUAL(write_usize(buffer, NUMBER, optsDef, count),       \
                            EXPECTED_COUNT);                                   \
            GSHL_TEST_STR_EQUAL(buffer, EXPECTED_STRING);                      \
        }

    TEST_WRITE_usize(0, 1, "0");
    TEST_WRITE_usize(3245, 4, "3245");
    TEST_WRITE_usize(34948239, 8, "34948239");
    // TEST_WRITE_usize(usize_MAX, 10, "4294967295");
    // TEST_WRITE_usize(usize_MIN, 1, "0");

#    undef TEST_WRITE_usize
}

#endif
