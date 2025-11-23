#include "string/mod.h"
#include <stdlib.h>
#include <string.h>

char *string_dup(const char *const source)
{
    const usize len = strlen(source);
    return strcpy(malloc(len + 1), source);
}
