#include "format/pointer.h"

#include "macros/mod.h"
#include "string/mod.h"

#include <assert.h>
#include <string.h>

usize write_pointer(char *buf, GSHL_Template *t_mut)
{
    const GSHL_Template *const t = t_mut;

    GSHL_ASSERT(t->kind == GSHL_TEMPLATE_POINTER);

    static const char prefix[] = "0x";
    static const usize prefixLen = GSHL_STACK_STRING_LEN(prefix);

    if (buf == NULL) {
        if (t->pointer == NULL) {
            return t_mut->count = GSHL_STACK_STRING_LEN("nil");
        }

        t_mut->count = prefixLen;

        uintptr_t address = (uintptr_t)t->pointer;
        do {
            address >>= 4;
            t_mut->count += 1;
        } while (address != 0);

        return t->count;
    }

    if (t->pointer == NULL) {
        GSHL_ASSERT(t->count == 3);
        memcpy(buf, "nil", t->count);
        return t->count;
    }

    uintptr_t address = (uintptr_t)t->pointer;

    static const char conversion[][17] = {
        [false] = "0123456789abcdef",
        [true] = "0123456789ABCDEF",
    };

    // Fill the buffer with hexadecimal representation
    for (isize i = t->count - prefixLen - 1; i >= 0 && address > 0;
         --i, address >>= 4) {
        buf[prefixLen + i] =
            conversion[t->opts.pointer.uppercase][address % 16];
    }

    memcpy(buf, prefix, prefixLen);

    return t->count;
}

#ifdef GSHL_TESTS

#    include "test/mod.h"

GSHL_TEST(write_pointer)
{
#    define TEST_WRITE_POINTER(NUMBER, EXPECTED_STRING, ...)                   \
        {                                                                      \
            char buffer[256] = {0};                                            \
            struct GSHL_Template template = {                                  \
                .kind = GSHL_TEMPLATE_POINTER,                                 \
                .pointer = NUMBER,                                             \
                .opts.pointer = {__VA_ARGS__},                                 \
            };                                                                 \
            const usize count = GSHL_STACK_STRING_LEN(EXPECTED_STRING);        \
            GSHL_TEST_EQUAL(write_pointer(NULL, &template), count);            \
            GSHL_TEST_EQUAL(write_pointer(buffer, &template), count);          \
            GSHL_TEST_STR_EQUAL(buffer, EXPECTED_STRING);                      \
        }

    TEST_WRITE_POINTER(0, "nil");
    TEST_WRITE_POINTER(NULL, "nil");
    TEST_WRITE_POINTER((void *)0x7ffc0d1b64f4, "0x7ffc0d1b64f4");

#    undef TEST_WRITE_POINTER
}

#endif
