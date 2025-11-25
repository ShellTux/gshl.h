#define GSHL_IMPLEMENTATION
#define GSHL_STRIP_PREFIX
#include "../gshl.h"

int main(void)
{
    STRING_FOREACH("Hello world!", c) { print("%c", c); }
    println("");

    return 0;
}
