#include "string/mod.h"
#include <stdlib.h>
#include <string.h>

char *GSHL_string_dup(const char *const source)
{
    const usize len = strlen(source);
    return strcpy(malloc(len + 1), source);
}

void GSHL_string_reverse(char *restrict start, char *restrict end)
{
    end -= 1;

    while (start < end) {
        char temp = *start;
        *start = *end;
        *end = temp;

        start++;
        end--;
    }
}

#ifdef GSHL_TESTS
#    include "test/mod.h"

GSHL_TEST(string_dup)
{
    {
        char *s = GSHL_string_dup("example");
        GSHL_TEST_STR_EQUAL(s, "example");
        free(s);
    }

    {
        char *s = GSHL_string_dup("Hello world!");
        GSHL_TEST_STR_EQUAL(s, "Hello world!");
        free(s);
    }
}

GSHL_TEST(string_reverse)
{
    {
        char s1[] = "123456789";
        char s2[] = "123456789";
        GSHL_string_reverse(s2, s2 + GSHL_STACK_STRING_LEN(s2));
        GSHL_TEST_STR_EQUAL(s2, "987654321");
        GSHL_string_reverse(s2, s2 + GSHL_STACK_STRING_LEN(s2));
        GSHL_TEST_STR_EQUAL(s1, s2);
    }
}

#endif
