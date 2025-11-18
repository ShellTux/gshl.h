#include "array/mod.h"
#include "types/mod.h"
#include <stdlib.h>

#ifdef GSHL_TESTS

#    include "test/mod.h"

GSHL_TEST(stackarray)
{
    const u8 a[] = {1, 2, 3, 4, 5};

    GSHL_TEST_EQUAL(GSHL_ARRAY_LEN(a), 5);

    u8 sum = 0;
    GSHL_ARRAY_FOREACH(a, u8 element) { sum += element; }
    GSHL_TEST_EQUAL(sum, 15);
}

GSHL_TEST(array)
{
    const struct {
        i32 *items;
        usize count;
    } array = {
        .count = 5,
        .items = calloc(5, sizeof(i32)),
    };

    GSHL_ARRAYN_FOREACH_MUT(array.items, array.count, i32 * element)
    {
        *element = index + 1;
    }

    u8 sum = 0;
    GSHL_ARRAYN_FOREACH(array.items, array.count, i32 element)
    {
        sum += element;
    }
    GSHL_TEST_EQUAL(sum, 15);

    free(array.items);
}

#endif
