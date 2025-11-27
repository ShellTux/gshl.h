#include "log/mod.h"

#include "ansi/colors.h"
#include "array/mod.h"
#include "format/mod.h"
#include "macros/mod.h"
#include "math/mod.h"
#include "print/mod.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static GSHL_LogConfig log_config = {
    .fd = STDERR_FILENO,
    .mask = GSHL_LOG_ALL,
    .file_fd = 0,
    .print =
        {
            .file = true,
            .function = GSHL_LOG_PRINT_FUNCTION,
        },
};

usize write_enum_LogKind(struct GSHL_FormatString *string,
                         const struct GSHL_FormatSpecifier *fs)
{
    GSHL_ASSERT(fs != NULL);

    GSHL_LogKind log = fs->value.u32;

    if (log == 0) {
        return GSHL_format_write(string, "NONE");
    }

    usize count = 0;
    for (GSHL_LogKind mask = 1U << 0; log > 0; log &= ~mask, mask <<= 1) {
        const GSHL_LogKind enabled_log = log & mask;

        if (count > 0 && enabled_log) {
            count += GSHL_format_write(string, " | ");
        }

        switch (enabled_log) {
#define VERBOSITY(ENUM, BIT_SHIFT, FORMAT, ...)                                \
    case GSHL_LOG_##ENUM: {                                                    \
        count += GSHL_format_write(string, FORMAT, #ENUM);                     \
    } break;
            GSHL_LOG_VERBOSITY_LEVELS
#undef VERBOSITY
        default:
            break;
        }
    }

    return count;
}

usize write_struct_LogConfig(struct GSHL_FormatString *string,
                             const struct GSHL_FormatSpecifier *fs)
{
    GSHL_ASSERT(fs != NULL);
    const GSHL_LogConfig *config = fs->value.pointer;

    return GSHL_format_write(string,
                             "struct LogConfig {{\n"
                             "  .fd = %i\n"
                             "  .mask = {enum GSHL_LogKind}\n"
                             "  .file_fd = %i\n"
                             "  .print = {{\n"
                             "    .function = %u\n"
                             "    .file = %B\n"
                             "  }}\n"
                             "}}",
                             config->fd, config->mask, config->file_fd,
                             (u32)config->print.function, config->print.file);
}

void GSHL_log_init_wrapper(const GSHL_LogConfig config)
{
    static const GSHL_LogConfig null_config = {0};
    if (config.fd != null_config.fd) {
        if (config.fd == -1) {
            perror("log_config.fd");
            return;
        }

        log_config.fd = config.fd;
    }

    if (config.mask != null_config.mask) {
        log_config.mask = config.mask;
        GSHL_log_read_env();
    }

    if (config.file_fd != null_config.file_fd) {
        if (config.file_fd == -1) {
            perror("log_config.file_fd");
            return;
        }

        log_config.file_fd = config.file_fd;
    }

    if (log_config.print.file != config.print.file) {
        log_config.print.file = config.print.file;
    }

    if (log_config.print.function != config.print.function) {
        log_config.print.function = config.print.function;
    }

    GSHL_ASSERT(log_config.fd > 0);

    GSHL_format_specifier_register((GSHL_FormatSpecifier){
        .kind = GSHL_FORMAT_SPECIFIER_U32,
        .va_size = sizeof(GSHL_LogKind),
        .write = write_enum_LogKind,
        .specifiers = {"enum GSHL_LogKind", "GSHL_LogKind"},
    });

    GSHL_format_specifier_register((GSHL_FormatSpecifier){
        .kind = GSHL_FORMAT_SPECIFIER_POINTER,
        .va_size = sizeof(GSHL_LogConfig *),
        .write = write_struct_LogConfig,
        .specifiers = {"struct GSHL_LogConfig", "GSHL_LogConfig"},
    });
}

GSHL_LogConfig GSHL_log_get_config(void) { return log_config; }

bool GSHL_log_is_allowed(const GSHL_LogKind kind)
{
    GSHL_ASSERT(GSHL_is_power_of_two(kind) || kind == GSHL_LOG_NONE);

    return (kind & log_config.mask) != GSHL_LOG_NONE;
}

usize GSHL_log_wrapper(const GSHL_LogKind kind, const GSHL_LogOpts opts,
                       const char *const restrict format, ...)
{
    if (!GSHL_log_is_allowed(kind)) {
        return 0;
    }

    if (log_config.fd <= 0) {
        GSHL_dprintln(STDERR_FILENO, "log_config.fd = %i", log_config.fd);
        GSHL_dprintln(STDERR_FILENO, "log_config.file_fd = %i",
                      log_config.file_fd);
        return 0;
    }

    GSHL_FormatString string = {};
    GSHL_DArray_init(&string);

    static const char iden[] = "  ";
    static const usize idenLen = GSHL_STACK_STRING_LEN(iden);
    GSHL_format_write(&string, "%s[{enum GSHL_LogKind}]: ", iden, kind);

    if (log_config.print.file) {
        GSHL_format_write(&string, "%s:%lu > ", opts.file, opts.line);
    }

    switch (log_config.print.function) {
    case GSHL_LOG_PRINT_FUNCTION:
        GSHL_format_write(&string, "%s > ", opts.function);
        break;
    case GSHL_LOG_PRINT_PRETTY_FUNCTION:
        GSHL_format_write(&string, "%s > ", opts.pretty_function);
        break;
    default:
        GSHL_UNREACHABLE("Unknown log_config.print.function = %i",
                         log_config.print.function);
        break;
    }

    va_list args;
    va_start(args, format);
    GSHL_format_writev(&string, format, args);
    va_end(args);

    GSHL_DArray_append(&string, '\n');
    GSHL_DArray_append(&string, '\0');

    usize count = 0;
    if (log_config.file_fd > 0) {
        count = write(log_config.file_fd, string.items + idenLen,
                      string.len - idenLen - 1);
    }

    if (log_config.fd > 0) {
        count = write(log_config.fd, string.items, string.len - 1);
    }

    GSHL_ASSERT(count > 0);

    GSHL_DArray_destroy(&string);

    return count;
}

void GSHL_log_read_env(void)
{
    static const struct GSHL_Env2Log {
        char *values[2];
        GSHL_LogKind kind;
    } conversions[] = {
#define VERBOSITY(ENUM, BIT_SHIFT, FORMAT, ...)                                \
    {.values = {__VA_ARGS__}, .kind = GSHL_LOG_##ENUM},
        GSHL_LOG_VERBOSITY_LEVELS
#undef VERBOSITY

    };

    const char *const verbosity = getenv("GSHL_LOG");
    if (verbosity == NULL) {
        return;
    }

    log_config.mask = 0;
    GSHL_ARRAY_FOREACH(conversions, struct GSHL_Env2Log env2log)
    {
        GSHL_ARRAY_FOREACH(env2log.values, char *value)
        {
            if (strstr(verbosity, value) == NULL) {
                continue;
            }

            log_config.mask |= env2log.kind;
        }
    }
}
