#include "format/char.h"

#include "macros/mod.h"
#include "string/mod.h"
#include "types/mod.h"

#include <assert.h>
#include <string.h>

usize write_character(char *buf, GSHL_Template *t_mut)
{
    const GSHL_Template *const t = t_mut;

    GSHL_ASSERT(t->kind == GSHL_TEMPLATE_CHAR);

    if (buf == NULL) {
        return t_mut->count = t->character != '\0';
    }

    GSHL_ASSERT((t->character == '\0' && t->count == 0) ||
                (t->character != '\0' && t->count == 1));
    buf[0] = t->character;

    return t->count;
}

#ifdef GSHL_TESTS

#    include "test/mod.h"

GSHL_TEST(write_char)
{
#    define TEST_WRITE_CHAR(CHAR, EXPECTED_STRING, ...)                        \
        {                                                                      \
            char buffer[256] = {0};                                            \
            struct GSHL_Template template = {                                  \
                .kind = GSHL_TEMPLATE_CHAR,                                    \
                .character = CHAR,                                             \
                .opts.character = {__VA_ARGS__},                               \
            };                                                                 \
            const usize count = GSHL_STACK_STRING_LEN(EXPECTED_STRING);        \
            GSHL_TEST_EQUAL(write_character(NULL, &template), count);          \
            GSHL_TEST_EQUAL(write_character(buffer, &template), count);        \
            GSHL_TEST_STR_EQUAL(buffer, EXPECTED_STRING);                      \
        }

    TEST_WRITE_CHAR(0, "");
    TEST_WRITE_CHAR('0', "0");
    TEST_WRITE_CHAR('1', "1");
    TEST_WRITE_CHAR('2', "2");
    TEST_WRITE_CHAR('3', "3");
    TEST_WRITE_CHAR('4', "4");
    TEST_WRITE_CHAR('5', "5");
    TEST_WRITE_CHAR('6', "6");
    TEST_WRITE_CHAR('7', "7");
    TEST_WRITE_CHAR('8', "8");
    TEST_WRITE_CHAR('9', "9");

#    undef TEST_WRITE_CHAR
}

#endif
