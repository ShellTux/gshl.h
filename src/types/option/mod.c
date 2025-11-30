#include "types/option/mod.h"

#include "format/write.h"
#include "print/mod.h"

#include <stdlib.h>
#include <unistd.h>

#define GSHL_TYPE(ALIAS, TYPE) OptionDecl(ALIAS);
GSHL_TYPES
#undef GSHL_TYPE

bool Option_is_some_or_panic(const enum OptionVariants variant,
                             const char *const restrict format, ...)
{
    if (variant == GSHL_NoneTag) {
        va_list args;
        va_start(args, format);
        GSHL_dprintlnv(STDERR_FILENO, format, args);
        va_end(args);
        exit(EXIT_FAILURE);
        return false;
    }

    return true;
}
