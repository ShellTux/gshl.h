#define GSHL_IMPLEMENTATION
#define GSHL_STRIP_PREFIX
#define GSHL_GLOBAL_FORMAT_STRING
#include "../gshl.h"

#define N 5e5

int main(void)
{
    for (usize i = 0; i < N; ++i) {
        for (usize number = 0; number <= 10; number += 1) {
            println("%lu! = %lu", number, factorial(number));
        }
    }

    for (usize i = 0; i < N; ++i) {
        for (usize number = 0; number <= 10; number += 1) {
            printf("%lu! = %lu\n", number, factorial(number));
        }
    }

    return 0;
}
