#define GSHL_IMPLEMENTATION
#define GSHL_STRIP_PREFIX
#include "../gshl.h"

int main(void)
{
    for (usize i = 0; i < 1e5; ++i) {
        for (usize number = 0; number <= 10; number += 1) {
            print("%lu! = %lu\n", number, factorial(number));
        }
    }

    for (usize i = 0; i < 1e5; ++i) {
        for (usize number = 0; number <= 10; number += 1) {
            printf("%lu! = %lu\n", number, factorial(number));
        }
    }

    return 0;
}
