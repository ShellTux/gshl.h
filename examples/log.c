#define GSHL_STRIP_PREFIX
#define GSHL_IMPLEMENTATION
#include "../gshl.h"

#include <fcntl.h>
#include <unistd.h>

int main(void)
{
    log_init(.fd = STDERR_FILENO, .mask = GSHL_LOG_INFO | GSHL_LOG_WARNING,
             .print.file = true,
             // O_WRONLY: Open for writing only
             // O_CREAT: Create the file if doesn't exist
             // O_TRUNC: Truncate the file (Overwrite existing content)
             // O_APPEND: Append to the file
             .file_fd =
                 open("example.log", O_WRONLY | O_CREAT | O_TRUNC, 0644));
    println("{struct GSHL_LogConfig}", &log_config);

    log(GSHL_LOG_DEBUG, "debug message");
    log(GSHL_LOG_INFO, "%i %i", 6, 7);
    log(GSHL_LOG_WARNING, "%i + %i = %i", 34, 35, 34 + 35);
    log(GSHL_LOG_ERROR, "%s", "error message");

    return 0;
}
