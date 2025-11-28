#include "print/mod.h"
#include "format/mod.h"
#include "string/mod.h"

#include <stdarg.h>
#include <unistd.h>

#ifdef GSHL_GLOBAL_FORMAT_STRING
#    include "array/mod.h"
#    include "macros/mod.h"

#    include <assert.h>
#    include <stdlib.h>
#    include <string.h>

static GSHL_GlobalFormatString global_string = {};

extern int usleep(__useconds_t usec);

static void *GSHL_global_format_string_writer_thread(void *arg)
{
    GSHL_UNUSED(arg);

    while (true) {
        pthread_mutex_lock(&global_string.lock);
        {
            while (!global_string.ready) {
                pthread_cond_wait(&global_string.notify, &global_string.lock);
            }

            for (usize fd = STDOUT_FILENO;
                 fd < GSHL_ARRAY_LEN(global_string.buffers); ++fd) {
                GSHL_FormatString *string = &global_string.buffers[fd].string;

                GSHL_ASSERT(fd > 0);

                if (string->count == 0) {
                    continue;
                }

                assert(write(fd, string->items, string->count) > 0);
                GSHL_DArray_init(string);
            }

            // Reset the buffer state
            global_string.ready = false;
        }
        pthread_mutex_unlock(&global_string.lock);

        usleep(10 * 1000);
    }

    return NULL;
}

static __attribute__((constructor)) void
GSHL_global_format_string_constructor(void)
{
    pthread_mutex_init(&global_string.lock, NULL);
    pthread_cond_init(&global_string.notify, NULL);
    pthread_create(&global_string.writer_tid, NULL,
                   GSHL_global_format_string_writer_thread, NULL);

    pthread_mutex_lock(&global_string.lock);
    for (usize fd = STDOUT_FILENO; fd < GSHL_ARRAY_LEN(global_string.buffers);
         ++fd) {
        GSHL_FormatString *string = &global_string.buffers[fd].string;
        GSHL_DArray_init(string);

        GSHL_ASSERT(fd > 0);
    }
    pthread_mutex_unlock(&global_string.lock);
}

static __attribute__((destructor)) void
GSHL_global_format_string_destructor(void)
{
    pthread_mutex_lock(&global_string.lock);
    for (usize fd = STDOUT_FILENO; fd < GSHL_ARRAY_LEN(global_string.buffers);
         ++fd) {
        GSHL_FormatString *string = &global_string.buffers[fd].string;

        GSHL_ASSERT(fd > 0);

        if (string->count > 0) {
            assert(write(fd, string->items, string->count) > 0);
        }

        GSHL_DArray_destroy(string);
    }
    pthread_mutex_unlock(&global_string.lock);

    // Cleanup
    pthread_cancel(global_string.writer_tid);
    pthread_join(global_string.writer_tid, NULL);

    pthread_mutex_destroy(&global_string.lock);
    pthread_cond_destroy(&global_string.notify);
}
#endif

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
    if (fd <= 0) {
        return 0;
    }

#ifdef GSHL_GLOBAL_FORMAT_STRING
    pthread_mutex_lock(&global_string.lock);
    usize count = 0;
    for (usize i = STDOUT_FILENO; i < GSHL_ARRAY_LEN(global_string.buffers);
         ++i) {
        global_string.buffers[i].fd = i;
        GSHL_FormatString *string = &global_string.buffers[i].string;
        const Fd global_fd = global_string.buffers[i].fd;

        GSHL_ASSERT(global_fd > 0);

        if (global_fd != fd) {
            continue;
        }

        const usize start_count = string->count;
        GSHL_format_writev(string, format, args);
        count = string->count - start_count;
        global_string.ready = true;                 // Mark buffer as ready
        pthread_cond_signal(&global_string.notify); // Signal writer thread

        break;
    }
    pthread_mutex_unlock(&global_string.lock);

    if (count != 0) {
        return count;
    }
#endif
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
    if (fd <= 0) {
        return 0;
    }

#ifdef GSHL_GLOBAL_FORMAT_STRING
    pthread_mutex_lock(&global_string.lock);
    usize count = 0;
    for (usize i = STDOUT_FILENO; i < GSHL_ARRAY_LEN(global_string.buffers);
         ++i) {
        global_string.buffers[i].fd = i;
        GSHL_FormatString *string = &global_string.buffers[i].string;
        const Fd global_fd = global_string.buffers[i].fd;

        GSHL_ASSERT(global_fd > 0);

        if (global_fd != fd) {
            continue;
        }

        const usize start_count = string->count;
        GSHL_format_writev(string, format, args);
        GSHL_format_write(string, "\n");
        count = string->count - start_count;
        global_string.ready = true;                 // Mark buffer as ready
        pthread_cond_signal(&global_string.notify); // Signal writer thread

        break;
    }
    pthread_mutex_unlock(&global_string.lock);

    if (count != 0) {
        return count;
    }
#endif

    const GSHL_StringView view = GSHL_format_wrapperv(format, args);

    return write(fd, view.start, view.len) + write(fd, "\n", 1);
}
