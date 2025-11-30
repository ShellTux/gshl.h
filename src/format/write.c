#include "format/write.h"

#include "format/mod.h"
#include "hash_table/mod.h"

#include <assert.h>
#include <stdlib.h>

extern __thread GSHL_HashTable GSHL_format_ht;

usize GSHL_format_write(GSHL_FormatString *string,
                        const char *const restrict format, ...)
{
    va_list args;
    va_start(args, format);
    const usize count = GSHL_format_writev(string, format, args);
    va_end(args);
    return count;
}

usize GSHL_format_writev(GSHL_FormatString *string,
                         const char *const restrict format, va_list args)
{
    const char *formatEnd = format;

    for (const char *formatC = format; *formatC != '\0'; formatC += 1) {
        formatEnd = formatC;

        if ((formatC[0] == '{' && formatC[1] == '{') ||
            (formatC[0] == '}' && formatC[1] == '}')) {
            GSHL_DArray_append(string, formatC[1]);
            formatC += 1;
            continue;
        }

        const usize fs_hash =
            GSHL_hash_format_specifier(formatC, NULL, &formatEnd);

        if (formatC == formatEnd) {
            GSHL_DArray_append(string, *formatC);
        }
        else {
            const GSHL_HashTableValue *valueP =
                GSHL_HashTable_search(&GSHL_format_ht, .usize = fs_hash);
            if (valueP == NULL) {
                fprintf(stderr, "format: \"%s\" \"%.*s\"\n", format,
                        (int)(formatEnd - formatC), formatC);
                fprintf(stderr, "hash: %lu\n", fs_hash);
                fprintf(stderr, "valueP: %p\n", valueP);
                fprintf(stderr, "Unknown format specifier '%.*s'\n",
                        (int)(formatEnd - formatC), formatC);

                for (; formatC < formatEnd; formatC += 1) {
                    GSHL_DArray_append(string, *formatC);
                }

                formatC = formatEnd - 1;
                continue;
            }

            GSHL_FormatSpecifier fs = *(GSHL_FormatSpecifier *)valueP->opaque;

            GSHL_ASSERT(fs.write != NULL);
            GSHL_ASSERT(fs.value.usize == 0);

            switch (fs.va_size) {
            case 4:
                fs.value.u32 = va_arg(args, u32);
                break;
            case 8:
                fs.value.u64 = va_arg(args, u64);
                break;
            default:
                GSHL_UNREACHABLE("Wrong fs->va_size = %lu", fs.va_size);
                break;
            }

            fs.write(string, &fs);

            formatC = formatEnd - 1;
        }
    }

    GSHL_ASSERT(format <= formatEnd);

    return (usize)(formatEnd - format);
}
