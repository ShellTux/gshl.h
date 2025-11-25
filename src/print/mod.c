#include "print/mod.h"
#include "format/mod.h"
#include "string/mod.h"

#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>

usize GSHL_print(const char *const restrict format, ...)
{
    va_list args;
    va_start(args, format);
    const usize count = GSHL_printv(format, args);
    va_end(args);
    return count;
}

usize GSHL_dprint(const int fd, const char *const restrict format, ...)
{
    va_list args;
    va_start(args, format);
    const usize count = GSHL_dprintv(fd, format, args);
    va_end(args);
    return count;
}

usize GSHL_println(const char *const restrict format, ...)
{
    va_list args;
    va_start(args, format);
    const usize count = GSHL_printlnv(format, args);
    va_end(args);
    return count;
}

usize GSHL_dprintln(const int fd, const char *const restrict format, ...)
{
    va_list args;
    va_start(args, format);
    const usize count = GSHL_dprintlnv(fd, format, args);
    va_end(args);
    return count;
}

usize GSHL_printv(const char *const restrict format, va_list args)
{
    return GSHL_dprintv(STDOUT_FILENO, format, args);
}

usize GSHL_dprintv(const int fd, const char *const restrict format,
                   va_list args)
{
    const GSHL_StringView view = GSHL_format_wrapperv(format, args);

    return write(fd, view.start, view.len);
}

usize GSHL_printlnv(const char *const restrict format, va_list args)
{
    return GSHL_dprintlnv(STDOUT_FILENO, format, args);
}

usize GSHL_dprintlnv(const int fd, const char *const restrict format,
                     va_list args)
{
    const GSHL_StringView view = GSHL_format_wrapperv(format, args);

    return write(fd, view.start, view.len) + write(fd, "\n", 1);
}
