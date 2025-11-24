#define GSHL_IMPLEMENTATION
#define GSHL_STRIP_PREFIX
#include "../gshl.h"

#define SIZE 512

int main(void)
{
    const char *strings[] = {
#define KIND(ENUM, NAME, TYPE, VA_ARG_TYPE, BIT_SHIFT, OPTS, ...) "{" #NAME "}",
        GSHL_TEMPLATE_KINDS
#undef KIND
        "%lu",
        "%u",
        "%li",
        "%i",
    };

    // ARRAY_FOREACH(strings, char *string)
    // {
    //     const usize h = custom_hash_kr(string);
    //     fprintf(stderr, "%20s = %10lu\n", string, h);
    //     fprintf(stdout, "%-20lu = %s\n", h, string);
    // }

    ARRAY_FOREACH(strings, char *string)
    {
        const u32 h = hash_fnv1a32(string) % SIZE;
        fprintf(stderr, "%20s = %10u\n", string, h);
        fprintf(stdout, "%-20u = %s\n", h, string);
    }

    return EXIT_SUCCESS;
}
