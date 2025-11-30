#define GSHL_IMPLEMENTATION
#define GSHL_STRIP_PREFIX
#include "../gshl.h"

#define Option_match(A) switch ((A).variant)

Option(usize) fact(const isize n)
{
    if (n < 0) {
        return None(usize);
    }

    if (n <= 1) {
        return Some(usize, 1);
    }

    return Some(usize, n * factorial(n - 1));
}

int main(void)
{
    {
        const Option(i32) a = Some(i32, 5);
        println("{Option(i32)}.unwrap() = {i32}", &a, Option_unwrap(a));
    }

    {
        const Option(i32) a = None(i32);
        println("{Option(i32)}.unwrap_or(0) = {i32}", &a,
                Option_unwrap_or(a, 0));
    }

    for (isize n = -2; n <= 5; n++) {
        const Option(usize) result = fact(n);

        if (result.is_some()) {
            println("{isize}! = {Option(usize)}.unwrap() = {usize}", n, &result,
                    Option_unwrap(result));
        }
        else {
            println("{isize}! = {Option(usize)}", n, &result);
        }
    }

    return 0;
}
