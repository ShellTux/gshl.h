#include "format/i16.h"
#include "format/isize.h"

#include "types/mod.h"

usize write_i16(char *buf, const i16 i16,
                const struct GSHL_TemplateOpts_i16 opts,
                const usize precomputed_count)
{
    const struct GSHL_TemplateOpts_isize new_opts = {};

    return write_isize(buf, i16, new_opts, precomputed_count);
}

#ifdef GSHL_TESTS

#    include "test/mod.h"

GSHL_TEST(write_i16)
{
#    define TEST_WRITE_I16(NUMBER, EXPECTED_COUNT, EXPECTED_STRING, ...)       \
        {                                                                      \
            char buffer[256] = {0};                                            \
            const struct GSHL_TemplateOpts_i16 optsDef = {__VA_ARGS__};        \
            const usize count = write_i16(NULL, NUMBER, optsDef, 0);           \
            GSHL_TEST_EQUAL(count, EXPECTED_COUNT);                            \
            GSHL_TEST_EQUAL(write_i16(buffer, NUMBER, optsDef, count),         \
                            EXPECTED_COUNT);                                   \
            GSHL_TEST_STR_EQUAL(buffer, EXPECTED_STRING);                      \
        }

    TEST_WRITE_I16(32767, 5, "32767");
#    undef TEST_WRITE_isize
}

#endif
