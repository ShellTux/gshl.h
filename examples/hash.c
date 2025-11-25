#define GSHL_IMPLEMENTATION
#define GSHL_STRIP_PREFIX
#include "../gshl.h"

#define SIZE 512

int main(void)
{
    const char *strings[] = {
        "Hello world",
        "Foo bar",
        "test",
    };

    println("");

    ARRAY_FOREACH(strings, char *string)
    {
        const usize djb2 = hash_djb2(string);
        const u32 fnv1a32 = hash_fnv1a32(string);
        const usize kr = hash_kr(string);
        println("djb2(\"{string}\") = {usize}", string, djb2);
        println("fnv1a32(\"{string}\") = {u32}", string, fnv1a32);
        println("kr(\"{string}\") = {usize}", string, kr);
        println("");
    }

    return EXIT_SUCCESS;
}
