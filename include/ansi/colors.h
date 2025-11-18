#ifndef INCLUDE_ANSI_COLORS_H_
#define INCLUDE_ANSI_COLORS_H_

/// {{{ Macros

#define GSHL_FG_BLACK(TEXT) "\033[30m" TEXT "\033[0m"
#define GSHL_FG_RED(TEXT) "\033[31m" TEXT "\033[0m"
#define GSHL_FG_GREEN(TEXT) "\033[32m" TEXT "\033[0m"
#define GSHL_FG_YELLOW(TEXT) "\033[33m" TEXT "\033[0m"
#define GSHL_FG_BLUE(TEXT) "\033[34m" TEXT "\033[0m"
#define GSHL_FG_MAGENTA(TEXT) "\033[35m" TEXT "\033[0m"
#define GSHL_FG_CYAN(TEXT) "\033[36m" TEXT "\033[0m"
#define GSHL_FG_WHITE(TEXT) "\033[37m" TEXT "\033[0m"

#define GSHL_BG_BLACK(TEXT) "\033[40m" TEXT "\033[0m"
#define GSHL_BG_RED(TEXT) "\033[41m" TEXT "\033[0m"
#define GSHL_BG_GREEN(TEXT) "\033[42m" TEXT "\033[0m"
#define GSHL_BG_YELLOW(TEXT) "\033[43m" TEXT "\033[0m"
#define GSHL_BG_BLUE(TEXT) "\033[44m" TEXT "\033[0m"
#define GSHL_BG_MAGENTA(TEXT) "\033[45m" TEXT "\033[0m"
#define GSHL_BG_CYAN(TEXT) "\033[46m" TEXT "\033[0m"
#define GSHL_BG_WHITE(TEXT) "\033[47m" TEXT "\033[0m"

/// }}}

/// {{{ Strip Prefix

#ifdef GSHL_STRIP_PREFIX
#    define FG_BLACK(...) GSHL_FG_BLACK(__VA_ARGS__)
#    define FG_RED(...) GSHL_FG_RED(__VA_ARGS__)
#    define FG_GREEN(...) GSHL_FG_GREEN(__VA_ARGS__)
#    define FG_YELLOW(...) GSHL_FG_YELLOW(__VA_ARGS__)
#    define FG_BLUE(...) GSHL_FG_BLUE(__VA_ARGS__)
#    define FG_MAGENTA(...) GSHL_FG_MAGENTA(__VA_ARGS__)
#    define FG_CYAN(...) GSHL_FG_CYAN(__VA_ARGS__)
#    define FG_WHITE(...) GSHL_FG_WHITE(__VA_ARGS__)

#    define BG_BLACK(...) GSHL_BG_BLACK(__VA_ARGS__)
#    define BG_RED(...) GSHL_BG_RED(__VA_ARGS__)
#    define BG_GREEN(...) GSHL_BG_GREEN(__VA_ARGS__)
#    define BG_YELLOW(...) GSHL_BG_YELLOW(__VA_ARGS__)
#    define BG_BLUE(...) GSHL_BG_BLUE(__VA_ARGS__)
#    define BG_MAGENTA(...) GSHL_BG_MAGENTA(__VA_ARGS__)
#    define BG_CYAN(...) GSHL_BG_CYAN(__VA_ARGS__)
#    define BG_WHITE(...) GSHL_BG_WHITE(__VA_ARGS__)
#endif

/// }}}

#endif // INCLUDE_ANSI_COLORS_H_
