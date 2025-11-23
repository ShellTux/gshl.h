#include "print/mod.h"
#include "format/mod.h"

#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>

static const struct GSHL_PrintWrapperOpts defPrintOpts = {.fd = STDOUT_FILENO};

usize GSHL_print_wrapperv(const GSHL_PrintWrapperOpts print_opts,
                          const GSHL_FormatWrapperOpts format_opts,
                          const char *restrict format, va_list args)
{
    struct GSHL_FormatWrapperResult result =
        GSHL_format_wrapperv(format, format_opts, args);

    if (result.templates.items != NULL) {
        free(result.templates.items);
        result.templates.items = NULL;
        result.templates.count = 0;
    }

    const usize count = write(print_opts.fd, result.string, result.count);

    GSHL_free_FormatWrapperResult(&result);

    return count;
}

usize GSHL_print(const char *restrict format, ...)
{
    static const GSHL_PrintWrapperOpts print_opts = {.fd = STDOUT_FILENO};
    static const GSHL_FormatWrapperOpts format_opts = {};

    va_list args;
    va_start(args, format);
    const usize count =
        GSHL_print_wrapperv(print_opts, format_opts, format, args);
    va_end(args);

    return count;
}

usize GSHL_dprint(const int fd, const char *restrict format, ...)
{
    const GSHL_PrintWrapperOpts print_opts = {.fd = fd};
    static const GSHL_FormatWrapperOpts format_opts = {};

    va_list args;
    va_start(args, format);
    const usize count =
        GSHL_print_wrapperv(print_opts, format_opts, format, args);
    va_end(args);

    return count;
}

usize GSHL_eprint(const char *restrict format, ...)
{
    static const GSHL_PrintWrapperOpts print_opts = {.fd = STDERR_FILENO};
    static const GSHL_FormatWrapperOpts format_opts = {};

    va_list args;
    va_start(args, format);
    const usize count =
        GSHL_print_wrapperv(print_opts, format_opts, format, args);
    va_end(args);

    return count;
}

usize GSHL_println(const char *restrict format, ...)
{
    static const GSHL_FormatWrapperOpts format_opts = {.suffix = "\n"};
    static const GSHL_PrintWrapperOpts print_opts = {.fd = STDOUT_FILENO};

    va_list args;
    va_start(args, format);
    const usize count =
        GSHL_print_wrapperv(print_opts, format_opts, format, args);
    va_end(args);

    return count;
}

usize GSHL_dprintln(const int fd, const char *restrict format, ...)
{
    static const GSHL_FormatWrapperOpts format_opts = {.suffix = "\n"};
    const GSHL_PrintWrapperOpts print_opts = {.fd = fd};

    va_list args;
    va_start(args, format);
    const usize count =
        GSHL_print_wrapperv(print_opts, format_opts, format, args);
    va_end(args);

    return count;
}

usize GSHL_eprintln(const char *restrict format, ...)

{
    static const GSHL_FormatWrapperOpts format_opts = {.suffix = "\n"};
    static const GSHL_PrintWrapperOpts print_opts = {.fd = STDERR_FILENO};

    va_list args;
    va_start(args, format);
    const usize count =
        GSHL_print_wrapperv(print_opts, format_opts, format, args);
    va_end(args);

    return count;
}
