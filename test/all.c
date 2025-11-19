#define GSHL_IMPLEMENTATION
#define GSHL_STRIP_PREFIX
#define GSHL_SOURCE_CODE_MAPPING
#ifndef GSHL_TESTS
#    define GSHL_TESTS
#endif
#include "../gshl.h"

int main(int argc, char *argv[])
{
    UNUSED(argc);
    UNUSED(argv);

    const int exit_code = run_tests(NULL) > 0;

    return exit_code;
}
