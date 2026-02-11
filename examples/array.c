#define GSHL_IMPLEMENTATION
#define GSHL_STRIP_PREFIX
#include "../gshl.h"

int main(void)
{
    // Stack array declaration
    {
        i32 array[] = {1, 2, 3};
        ARRAY_FOREACH(array, i32 e) { println("array[%lu] = %i", index, e); }
    }

    println("");

    // Another array declaration
    {
        Array(i32) array = {1, 2, 3};
        ARRAY_PRINT(array, "%i");
    }

    println("");

    {
        Array(i32, 5) array = {1, 2, 3};
        ARRAY_PRINT(array, "%i");
        // ARRAY_LEN works for stack arrays only
        println("array.length = %lu", ARRAY_LEN(array));
    }

    println("");

    // Nested array
    {
        Array(Array(i32, 3), 3) a = {
            {1, 2, 3},
            {4, 5, 6},
            {7, 8, 9},
        };

        for (usize i = 0; i < 3; ++i) {
            for (usize j = 0; j < 3; ++j) {
                print(" %i", a[i][j]);
            }
            println("");
        }
    }

    println("");

    return EXIT_SUCCESS;
}
