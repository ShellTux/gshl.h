#include "math/mod.h"

usize GSHL_factorial(const usize number)
{
    usize result = 1;
    for (usize n = 2; n <= number; ++n) {
        result *= n;
    }

    return result;
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

#endif
