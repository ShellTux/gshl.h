#include "string/mod.h"
#include <stdlib.h>
#include <string.h>

char *GSHL_string_dup(const char *const source)
{
    const usize len = strlen(source);
    return strcpy(malloc(len + 1), source);
}

#ifdef GSHL_TESTS
#    include "test/mod.h"

GSHL_TEST(string_dup)
{
    {
        char *s = string_dup("example");
        GSHL_TEST_STR_EQUAL(s, "example");
        free(s);
    }

    {
        char *s = string_dup("Hello world!");
        GSHL_TEST_STR_EQUAL(s, "Hello world!");
        free(s);
    }
}
#endif
