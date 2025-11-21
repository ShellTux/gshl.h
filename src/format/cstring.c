#include "format/cstring.h"

#include <string.h>

usize write_cstring(char *buf, const char *cstring,
                    const struct GSHL_TemplateOpts_cstring opts,
                    const usize precomputed_count)
{
    if (buf == NULL || precomputed_count == 0) {
        return strlen(cstring);
    }

    memcpy(buf, cstring, precomputed_count);

    return precomputed_count;
}

#ifdef GSHL_TESTS
#    include "test/mod.h"

GSHL_TEST(write_cstring)
{
#    define TEST_WRITE_CSTRING(CSTRING, EXPECTED_COUNT, EXPECTED_STRING, ...)  \
        {                                                                      \
            char buffer[256] = {0};                                            \
            const struct GSHL_TemplateOpts_cstring optsDef = {__VA_ARGS__};    \
            const usize count = write_cstring(NULL, CSTRING, optsDef, 0);      \
            GSHL_TEST_EQUAL(count, EXPECTED_COUNT);                            \
            GSHL_TEST_EQUAL(write_cstring(buffer, CSTRING, optsDef, count),    \
                            EXPECTED_COUNT);                                   \
            GSHL_TEST_STR_EQUAL(buffer, EXPECTED_STRING);                      \
        }

    TEST_WRITE_CSTRING("", 0, "");
    TEST_WRITE_CSTRING("abc", 3, "abc");
    TEST_WRITE_CSTRING("   4l45krjslkac  \n", 18, "   4l45krjslkac  \n");

#    undef TEST_WRITE_CSTRING
}

#endif
