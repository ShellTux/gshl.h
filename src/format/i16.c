#include "format/i16.h"
#include "format/isize.h"

#include "types/mod.h"

usize write_i16(char *buf, const i16 value, const usize precomputed_count)
{
    return write_isize(buf, value, precomputed_count);
}

#ifdef GSHL_TESTS

#    include "test/mod.h"

GSHL_TEST(write_i16)
{
#    define TEST_WRITE_I16(NUMBER, EXPECTED_COUNT, EXPECTED_STRING)            \
        {                                                                      \
            char buffer[256] = {0};                                            \
            const usize count = write_i16(NULL, NUMBER, 0);                    \
            GSHL_TEST_EQUAL(count, EXPECTED_COUNT);                            \
            GSHL_TEST_EQUAL(write_i16(buffer, NUMBER, count), EXPECTED_COUNT); \
            GSHL_TEST_STR_EQUAL(buffer, EXPECTED_STRING);                      \
        }

    TEST_WRITE_I16(32767, 5, "32767");
#    undef TEST_WRITE_isize
}

#endif
