#ifndef INCLUDE_LOG_MOD_H_
#define INCLUDE_LOG_MOD_H_

#include "format/mod.h"
#include "types/mod.h"

#include <stdarg.h>

/// {{{ Macros

#define GSHL_log(KIND, ...)                                                    \
    GSHL_log_wrapper(KIND,                                                     \
                     (GSHL_LogOpts){                                           \
                         .pretty_function = __PRETTY_FUNCTION__,               \
                         .function = __FUNCTION__,                             \
                         .file = __FILE__,                                     \
                         .line = __LINE__,                                     \
                     },                                                        \
                     __VA_ARGS__)

#define GSHL_log_init(...) GSHL_log_init_wrapper((GSHL_LogConfig){__VA_ARGS__})

/// }}}

/// {{{ Types

typedef enum GSHL_LogKind {
    GSHL_LOG_NONE = 0,
    GSHL_LOG_DEBUG = 1 << 0,
    GSHL_LOG_INFO = 1 << 1,
    GSHL_LOG_WARNING = 1 << 2,
    GSHL_LOG_ERROR = 1 << 3,

    GSHL_LOG_ALL =
        GSHL_LOG_DEBUG | GSHL_LOG_INFO | GSHL_LOG_WARNING | GSHL_LOG_ERROR,
} GSHL_LogKind;

typedef struct GSHL_LogOpts {
    const char *pretty_function;
    const char *function;
    const char *file;
    usize line;
} GSHL_LogOpts;

typedef struct GSHL_LogConfig {
    Fd fd;
    GSHL_LogKind mask;
    Fd file_fd;
    struct {
        enum GSHL_LogPrintFunction {
            GSHL_LOG_PRINT_FUNCTION = 0,
            GSHL_LOG_PRINT_PRETTY_FUNCTION = 1,
        } function : 1;
        bool file : 1;
    } print;
} GSHL_LogConfig;

/// }}}

/// {{{ Functions

void GSHL_log_init_wrapper(const GSHL_LogConfig config);
usize GSHL_log_wrapper(const GSHL_LogKind kind, const GSHL_LogOpts opts,
                       const char *const restrict format, ...);
GSHL_LogConfig GSHL_log_get_config(void);

/// }}}

#ifdef GSHL_STRIP_PREFIX
#    define LogKind GSHL_LogKind
#    define LogConfig GSHL_LogConfig
#    define log GSHL_log
#    define log_init GSHL_log_init
#    define log_get_config GSHL_log_get_config
#endif

#endif // INCLUDE_LOG_MOD_H_
