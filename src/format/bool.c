#include "format/bool.h"

#include "macros/mod.h"
#include "types/mod.h"

#include <assert.h>
#include <string.h>

usize write_boolean(char *buf, const bool boolean,
                    const usize precomputed_count)
{
    static const char *strings[] = {
        [false] = "false",
        [true] = "true",
    };

    static const usize lens[] = {
        [false] = sizeof("false") - 1,
        [true] = sizeof("true") - 1,
    };

    if (buf == NULL || precomputed_count == 0) {
        return lens[boolean];
    }

    GSHL_ASSERT(precomputed_count == lens[boolean]);

    memcpy(buf, strings[boolean], precomputed_count);

    return precomputed_count;
}

#ifdef GSHL_TESTS
#    include "test/mod.h"

GSHL_TEST(write_boolean)
{
#    define TEST_WRITE_BOOLEAN(NUMBER, EXPECTED_COUNT, EXPECTED_STRING)        \
        {                                                                      \
            char buffer[256] = {0};                                            \
            const usize count = write_boolean(NULL, NUMBER, 0);                \
            GSHL_TEST_EQUAL(count, EXPECTED_COUNT);                            \
            GSHL_TEST_EQUAL(write_boolean(buffer, NUMBER, count),              \
                            EXPECTED_COUNT);                                   \
            GSHL_TEST_STR_EQUAL(buffer, EXPECTED_STRING);                      \
        }

    TEST_WRITE_BOOLEAN(0, 5, "false");
    TEST_WRITE_BOOLEAN(1, 4, "true");
    TEST_WRITE_BOOLEAN(0b10101, 4, "true");
    TEST_WRITE_BOOLEAN(~0L, 4, "true");
    TEST_WRITE_BOOLEAN(true, 4, "true");
    TEST_WRITE_BOOLEAN(false, 5, "false");

#    undef TEST_WRITE_BOOLEAN
}

#endif
