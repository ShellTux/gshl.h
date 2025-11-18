#include "types/mod.h"

#ifdef GSHL_TESTS

#    include "test/mod.h"

GSHL_TEST(types_size)
{
    GSHL_TEST_EQUAL(sizeof(u8), 1);
    GSHL_TEST_EQUAL(sizeof(u16), 2);
    GSHL_TEST_EQUAL(sizeof(u32), 4);
    GSHL_TEST_EQUAL(sizeof(u64), 8);
    GSHL_TEST_EQUAL(sizeof(usize), 8);

    GSHL_TEST_EQUAL(sizeof(i8), 1);
    GSHL_TEST_EQUAL(sizeof(i16), 2);
    GSHL_TEST_EQUAL(sizeof(i32), 4);
    GSHL_TEST_EQUAL(sizeof(i64), 8);
    GSHL_TEST_EQUAL(sizeof(isize), 8);

    GSHL_TEST_EQUAL(sizeof(bool), 1);
}

GSHL_TEST(types_limits)
{
    GSHL_TEST_EQUAL((u8)(U8_MAX + 1), 0);
    GSHL_TEST_EQUAL((u8)(U8_MIN - 1), U8_MAX);

    GSHL_TEST_EQUAL((u16)(U16_MAX + 1), 0);
    GSHL_TEST_EQUAL((u16)(U16_MIN - 1), U16_MAX);

    GSHL_TEST_EQUAL((u32)(U32_MAX + 1), 0);
    GSHL_TEST_EQUAL((u32)(U32_MIN - 1), U32_MAX);
}

#endif
