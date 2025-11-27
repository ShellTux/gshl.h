#include "dynamic_array/mod.h"

#include "array/mod.h"

#include <assert.h>
#include <stdlib.h>

#ifdef GSHL_TESTS
#    include "test/mod.h"

GSHL_DArrayTypeDecl(DArray, int);

GSHL_TEST(da)
{
    DArray da = {};
    GSHL_TEST_EQUAL(da.items, NULL);
    GSHL_TEST_EQUAL(da.capacity, 0);
    GSHL_TEST_EQUAL(da.size, 0);

    GSHL_DArray_init(&da);
    GSHL_TEST_EQUAL(da.items, NULL);
    GSHL_TEST_EQUAL(da.capacity, 0);
    GSHL_TEST_EQUAL(da.size, 0);

    GSHL_DArray_append(&da, 10);
    GSHL_TEST_NEQUAL(da.items, NULL);
    GSHL_TEST_EQUAL(da.capacity, GSHL_DARRAY_INIT_CAPACITY);
    GSHL_TEST_EQUAL(da.size, 1);

    GSHL_DArray_append(&da, 67);
    GSHL_DArray_append(&da, 420);
    GSHL_DArray_append(&da, 69);
    GSHL_TEST_NEQUAL(da.items, NULL);
    GSHL_TEST_EQUAL(da.items[0], 10);
    GSHL_TEST_EQUAL(da.items[1], 67);
    GSHL_TEST_EQUAL(da.items[2], 420);
    GSHL_TEST_EQUAL(da.items[3], 69);
    GSHL_TEST_EQUAL(da.capacity, GSHL_DARRAY_INIT_CAPACITY);
    GSHL_TEST_EQUAL(da.size, 4);

    GSHL_DArray_insert(&da, 2, 12345);
    GSHL_TEST_NEQUAL(da.items, NULL);
    GSHL_TEST_EQUAL(da.items[0], 10);
    GSHL_TEST_EQUAL(da.items[1], 67);
    GSHL_TEST_EQUAL(da.items[2], 12345);
    GSHL_TEST_EQUAL(da.items[3], 420);
    GSHL_TEST_EQUAL(da.items[4], 69);
    GSHL_TEST_EQUAL(da.capacity, GSHL_DARRAY_INIT_CAPACITY);
    GSHL_TEST_EQUAL(da.size, 5);

    GSHL_DArray_remove(&da, 1);
    GSHL_TEST_NEQUAL(da.items, NULL);
    GSHL_TEST_EQUAL(da.items[0], 10);
    GSHL_TEST_EQUAL(da.items[1], 12345);
    GSHL_TEST_EQUAL(da.items[2], 420);
    GSHL_TEST_EQUAL(da.items[3], 69);
    GSHL_TEST_EQUAL(da.capacity, GSHL_DARRAY_INIT_CAPACITY);
    GSHL_TEST_EQUAL(da.size, 4);

    GSHL_DArray_init(&da);
    GSHL_TEST_NEQUAL(da.items, NULL);
    GSHL_TEST_NEQUAL(da.capacity, 0);
    GSHL_TEST_EQUAL(da.size, 0);

    for (usize n = 1; n <= 3 * GSHL_DARRAY_INIT_CAPACITY; n++) {
        GSHL_DArray_append(&da, n);
    }

    GSHL_TEST_NEQUAL(da.items, NULL);
    GSHL_TEST_EQUAL(da.capacity, 4 * GSHL_DARRAY_INIT_CAPACITY);
    GSHL_TEST_EQUAL(da.count, 3 * GSHL_DARRAY_INIT_CAPACITY);

    GSHL_DArray_destroy(&da);
    GSHL_TEST_EQUAL(da.items, NULL);
    GSHL_TEST_EQUAL(da.capacity, 0);
    GSHL_TEST_EQUAL(da.count, 0);

    const int elements[] = {1, 2, 3};
    GSHL_DArray_extend(&da, elements);
    GSHL_TEST_NEQUAL(da.items, NULL);
    GSHL_TEST_EQUAL(da.capacity, GSHL_DARRAY_INIT_CAPACITY);
    GSHL_TEST_EQUAL(da.count, GSHL_ARRAY_LEN(elements));
    GSHL_TEST_EQUAL(da.items[0], 1);
    GSHL_TEST_EQUAL(da.items[1], 2);
    GSHL_TEST_EQUAL(da.items[2], 3);

    const int elements2[] = {-1, -2, -3};
    GSHL_DArray_extend(&da, elements2);
    GSHL_TEST_NEQUAL(da.items, NULL);
    GSHL_TEST_EQUAL(da.capacity, GSHL_DARRAY_INIT_CAPACITY);
    GSHL_TEST_EQUAL(da.count,
                    GSHL_ARRAY_LEN(elements) + GSHL_ARRAY_LEN(elements2));
    GSHL_TEST_EQUAL(da.items[0], 1);
    GSHL_TEST_EQUAL(da.items[1], 2);
    GSHL_TEST_EQUAL(da.items[2], 3);
    GSHL_TEST_EQUAL(da.items[3], -1);
    GSHL_TEST_EQUAL(da.items[4], -2);
    GSHL_TEST_EQUAL(da.items[5], -3);

    GSHL_DArray_destroy(&da);
}

#endif
