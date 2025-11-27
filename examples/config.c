#define GSHL_IMPLEMENTATION
#include "config.h"
#include "../gshl.h"

int main(void)
{
    log_init();

    log(DEBUG, "Hello world!");

    return 0;
}
