#define GSHL_IMPLEMENTATION
#define GSHL_STRIP_PREFIX
#include "../gshl.h"

#include <fcntl.h>

int main(void)
{
    // NOTE: GSHL_LOG environment variable can be used to provide the verbosity
    // level:
    // GSHL_LOG=debug,info ./program
    log_init(.fd = STDERR_FILENO, .mask = INFO | WARNING, .print.file = true,
             // O_WRONLY: Open for writing only
             // O_CREAT: Create the file if doesn't exist
             // O_TRUNC: Truncate the file (Overwrite existing content)
             // O_APPEND: Append to the file
             .file_fd =
                 open("example.log", O_WRONLY | O_CREAT | O_TRUNC, 0644));
    println("{struct GSHL_LogConfig}", &log_config);

    log(DEBUG, "debug message");
    log(INFO, "%i %i", 6, 7);
    log(WARNING, "%i + %i = %i", 34, 35, 34 + 35);
    log(ERROR, "%s", "error message");

    return 0;
}
