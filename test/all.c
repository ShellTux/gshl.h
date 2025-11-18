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

    if (run_tests(NULL) > 0) {
        return 1;
    }

    return 0;
}
