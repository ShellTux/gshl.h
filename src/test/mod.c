#include "test/mod.h"
#include "macros/mod.h"
#include "types/mod.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static GSHL_Test *GSHL_test_list = NULL;

GSHLDEF usize GSHL_run_tests(const char *filter)
{
    usize failed_tests = 0;
    size_t total = 0;

    printf("Running tests...\n");

    // TODO: Remove O(n)
    GSHL_Test *last = NULL;
    for (last = GSHL_test_list; last != NULL && last->next != NULL;
         last = last->next) {
    }

    /* Reverse Order */
    for (GSHL_Test *current = last; current != NULL; current = current->prev) {
        if (filter != NULL && strstr(current->name, filter) == NULL) {
            continue;
        }

        current->func(&failed_tests);
        total++;
    }

    printf("\n");
    if (failed_tests != 0) {
        printf(GSHL_FG_RED("Failed tests:") " %zu\n", failed_tests);
    }
    printf("Total Tests:  %zu\n", total);

    return failed_tests;
}

GSHLDEF void GSHL_register_test_wrapper(const char *name,
                                        GSHL_TestFunction test_func)
{
    GSHL_Test *new_test = malloc(sizeof(*new_test));

    if (GSHL_test_list != NULL) {
        GSHL_test_list->prev = new_test;
    }

    new_test->name = (char *)name;
    new_test->func = test_func;
    new_test->next = GSHL_test_list;
    new_test->prev = NULL;

    GSHL_test_list = new_test;
}

int test_main(int argc, char *argv[])
{
    GSHL_UNUSED(argc);
    GSHL_UNUSED(argv);

    GSHL_run_tests(NULL);

    return 0;
}
