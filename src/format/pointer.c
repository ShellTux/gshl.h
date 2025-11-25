#include "format/pointer.h"

#include "format/hex.h"
#include "macros/mod.h"
#include "string/mod.h"
#include "types/mod.h"

#include <assert.h>
#include <stdlib.h>

usize GSHL_write_pointer(GSHL_FormatString *string,
                         const GSHL_FormatSpecifier *fs)
{
    GSHL_ASSERT(fs->kind == GSHL_FORMAT_SPECIFIER_POINTER);
    GSHL_ASSERT(fs->write == GSHL_write_pointer);

    usize count = 0;

    const pointer pointer = fs->value.pointer;
    if (pointer == NULL) {
        GSHL_STRING_FOREACH("nil", c)
        {
            if (string != NULL) {
                GSHL_DArray_append(string, c);
            }

            count += 1;
        }

        return count;
    }

    GSHL_STRING_FOREACH("0x", prefixC)
    {
        if (string != NULL) {
            GSHL_DArray_append(string, prefixC);
        }

        count += 1;
    }

    return count + GSHL_write_hex64(string, pointer);
}

#ifdef GSHL_TESTS
#    include "test/mod.h"

GSHL_TEST(write_pointer)
{

#    define TEST_WRITE_pointer(EXPRESSION, EXPECTED, ...)                      \
        do {                                                                   \
            GSHL_FormatString string = {};                                     \
            GSHL_DArray_init(&string);                                         \
            const GSHL_FormatSpecifier fs = {                                  \
                .kind = GSHL_FORMAT_SPECIFIER_POINTER,                         \
                .write = GSHL_write_pointer,                                   \
                .value.pointer = (EXPRESSION),                                 \
                .opts.pointer = {__VA_ARGS__},                                 \
            };                                                                 \
                                                                               \
            const usize count = GSHL_write_pointer(&string, &fs);              \
            const char expected[] = EXPECTED;                                  \
            GSHL_DArray_append(&string, '\0');                                 \
            GSHL_TEST_EQUAL(count, GSHL_STACK_STRING_LEN(EXPECTED));           \
            GSHL_TEST_STR_EQUAL(string.items, expected);                       \
            GSHL_DArray_destroy(&string);                                      \
        } while (0)

    TEST_WRITE_pointer(NULL, "nil");
    TEST_WRITE_pointer(NULL, "nil");

#    undef TEST_WRITE_pointer
}

#endif
