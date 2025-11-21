#include "format/mod.h"

#include <stdarg.h>
#include <stdlib.h>

char *GSHL_format(const char *restrict format, ...)
{
    va_list args;

    va_start(args, format);
    GSHL_FormatWrapperResult result = GSHL_format_wrapperv(format, args);
    va_end(args);

    if (result.templates.items != NULL) {
        free(result.templates.items);
        result.templates.items = NULL;
        result.templates.count = 0;
    }

    return result.string;
}
