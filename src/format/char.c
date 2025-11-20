#include "format/char.h"

#include "types/mod.h"

#include <assert.h>
#include <string.h>

usize write_character(char *buf, const char character,
                      const usize precomputed_count)
{
    if (buf == NULL || precomputed_count == 0) {
        return 1;
    }

    assert(precomputed_count == 1);
    buf[0] = character;

    return precomputed_count;
}

#ifdef GSHL_TESTS

#    include "test/mod.h"

GSHL_TEST(write_char)
{
#    define TEST_WRITE_CHAR(CHAR, EXPECTED_COUNT, EXPECTED_STRING)             \
        {                                                                      \
            char buffer[256] = {0};                                            \
            const usize count = write_character(NULL, CHAR, 0);                \
            GSHL_TEST_EQUAL(count, EXPECTED_COUNT);                            \
            GSHL_TEST_EQUAL(write_character(buffer, CHAR, count),              \
                            EXPECTED_COUNT);                                   \
            GSHL_TEST_STR_EQUAL(buffer, EXPECTED_STRING);                      \
        }

    TEST_WRITE_CHAR(0, 1, "");
    TEST_WRITE_CHAR('0', 1, "0");
    TEST_WRITE_CHAR('1', 1, "1");
    TEST_WRITE_CHAR('2', 1, "2");
    TEST_WRITE_CHAR('3', 1, "3");
    TEST_WRITE_CHAR('4', 1, "4");
    TEST_WRITE_CHAR('5', 1, "5");
    TEST_WRITE_CHAR('6', 1, "6");
    TEST_WRITE_CHAR('7', 1, "7");
    TEST_WRITE_CHAR('8', 1, "8");
    TEST_WRITE_CHAR('9', 1, "9");

#    undef TEST_WRITE_CHAR
}

#endif
