#include "print/mod.h"
#include "format/mod.h"

#include <stdarg.h>
#include <unistd.h>

usize GSHL_print(const char *restrict format, ...)
{
    va_list args;

    va_start(args, format);
    struct GSHL_FormatWrapperResult result = GSHL_format_wrapperv(format, args);
    va_end(args);

    const usize count = write(STDOUT_FILENO, result.string, result.count);

    GSHL_free_FormatWrapperResult(&result);

    return count;
}
