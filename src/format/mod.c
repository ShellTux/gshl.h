#include "format/mod.h"

#include <stdarg.h>
#include <stdlib.h>

char *GSHL_format(const char *restrict format, ...)
{
    va_list args;

    va_start(args, format);
    char *string = GSHL_formatv(format, args);
    va_end(args);

    return string;
}

char *GSHL_formatv(const char *restrict format, va_list args)
{
    static const GSHL_FormatWrapperOpts opts = {};

    GSHL_FormatWrapperResult result = GSHL_format_wrapperv(format, opts, args);

    if (result.templates.items != NULL) {
        free(result.templates.items);
        result.templates.items = NULL;
        result.templates.count = 0;
    }

    return result.string;
}

char *GSHL_formatln(const char *restrict format, ...)
{
    static const GSHL_FormatWrapperOpts opts = {
        .suffix = "\n",
    };

    va_list args;
    va_start(args, format);

    GSHL_FormatWrapperResult result = GSHL_format_wrapperv(format, opts, args);
    va_end(args);

    if (result.templates.items != NULL) {
        free(result.templates.items);
        result.templates.items = NULL;
        result.templates.count = 0;
    }

    return result.string;
}
