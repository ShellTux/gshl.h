#include "print/mod.h"
#include "format/mod.h"

#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>

usize GSHL_print(const char *restrict format, ...)
{
    va_list args;
    va_start(args, format);
    const usize count = GSHL_printv(format, args);
    va_end(args);

    return count;
}

usize GSHL_printv(const char *restrict format, va_list args)
{
    static const GSHL_FormatWrapperOpts opts = {};

    struct GSHL_FormatWrapperResult result =
        GSHL_format_wrapperv(format, opts, args);

    if (result.templates.items != NULL) {
        free(result.templates.items);
        result.templates.items = NULL;
        result.templates.count = 0;
    }

    const usize count = write(STDOUT_FILENO, result.string, result.count);

    GSHL_free_FormatWrapperResult(&result);

    return count;
}

usize GSHL_println(const char *restrict format, ...)
{
    static const GSHL_FormatWrapperOpts opts = {.suffix = "\n"};

    va_list args;
    va_start(args, format);
    struct GSHL_FormatWrapperResult result =
        GSHL_format_wrapperv(format, opts, args);
    va_end(args);

    if (result.templates.items != NULL) {
        free(result.templates.items);
        result.templates.items = NULL;
        result.templates.count = 0;
    }

    const usize count = write(STDOUT_FILENO, result.string, result.count);

    GSHL_free_FormatWrapperResult(&result);

    return count;
}
