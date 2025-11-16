#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define GSHL_STRIP_PREFIX
#include "../gshl.h"

int main(void)
{
    srand(time(NULL));

    switch (rand() % 2) {
    case 0: {
        TODO("Todo message");
    } break;
    case 1: {
        UNREACHABLE("Unreachable branch");
    } break;
    }

    {
        int i = 0;
        loop
        {
            if (i > 10) {
                break;
            }

            i++;
        }
    }

    return 0;
}
