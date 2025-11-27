#include "math/mod.h"

usize GSHL_factorial(const usize number)
{
    usize result = 1;
    for (usize n = 2; n <= number; ++n) {
        result *= n;
    }

    return result;
}

bool GSHL_is_power_of_two(const usize number)
{
    return (number > 0) && !(number & (number - 1));
}

#ifdef GSHL_TESTS

#    include "test/mod.h"

GSHL_TEST(factorial)
{
    GSHL_TEST_EQUAL(GSHL_factorial(0), 1);
    GSHL_TEST_EQUAL(GSHL_factorial(1), 1);
    GSHL_TEST_EQUAL(GSHL_factorial(2), 2);
    GSHL_TEST_EQUAL(GSHL_factorial(3), 6);
    GSHL_TEST_EQUAL(GSHL_factorial(4), 24);
    GSHL_TEST_EQUAL(GSHL_factorial(5), 120);
    GSHL_TEST_EQUAL(GSHL_factorial(6), 720);
    GSHL_TEST_EQUAL(GSHL_factorial(7), 5040);
}

GSHL_TEST(is_power_of_two)
{
    GSHL_TEST_EQUAL(GSHL_is_power_of_two(0), false);
    GSHL_TEST_EQUAL(GSHL_is_power_of_two(1), true);
    GSHL_TEST_EQUAL(GSHL_is_power_of_two(2), true);
    GSHL_TEST_EQUAL(GSHL_is_power_of_two(3), false);
    GSHL_TEST_EQUAL(GSHL_is_power_of_two(4), true);
    GSHL_TEST_EQUAL(GSHL_is_power_of_two(5), false);
    GSHL_TEST_EQUAL(GSHL_is_power_of_two(6), false);
    GSHL_TEST_EQUAL(GSHL_is_power_of_two(7), false);
    GSHL_TEST_EQUAL(GSHL_is_power_of_two(8), true);
    GSHL_TEST_EQUAL(GSHL_is_power_of_two(9), false);
    GSHL_TEST_EQUAL(GSHL_is_power_of_two(10), false);
    GSHL_TEST_EQUAL(GSHL_is_power_of_two(11), false);
    GSHL_TEST_EQUAL(GSHL_is_power_of_two(12), false);
    GSHL_TEST_EQUAL(GSHL_is_power_of_two(13), false);
    GSHL_TEST_EQUAL(GSHL_is_power_of_two(14), false);
    GSHL_TEST_EQUAL(GSHL_is_power_of_two(15), false);
    GSHL_TEST_EQUAL(GSHL_is_power_of_two(16), true);

    for (u8 i = 0; i < 64; ++i) {
        GSHL_TEST_EQUAL(GSHL_is_power_of_two(1UL << i), true);
    }
}

#endif
