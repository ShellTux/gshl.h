#define GSHL_IMPLEMENTATION
#define GSHL_STRIP_PREFIX
#include "../gshl.h"

int main(void)
{
    for (usize number = 0; number <= 10; number += 1) {
        println("{usize}! = %lu", number, factorial(number));
    }

    println("NULL = {pointer}", NULL);
    println("{string} %s", "Hello", "world!");

    return 0;
}
