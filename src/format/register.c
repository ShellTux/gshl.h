#include "format/register.h"

#include "format/bool.h"    // IWYU pragma: keep
#include "format/char.h"    // IWYU pragma: keep
#include "format/cstring.h" // IWYU pragma: keep
#include "format/hex.h"     // IWYU pragma: keep
#include "format/i16.h"     // IWYU pragma: keep
#include "format/i32.h"     // IWYU pragma: keep
#include "format/i64.h"     // IWYU pragma: keep
#include "format/i8.h"      // IWYU pragma: keep
#include "format/isize.h"   // IWYU pragma: keep
#include "format/pointer.h" // IWYU pragma: keep
#include "format/u16.h"     // IWYU pragma: keep
#include "format/u32.h"     // IWYU pragma: keep
#include "format/u64.h"     // IWYU pragma: keep
#include "format/u8.h"      // IWYU pragma: keep
#include "format/usize.h"   // IWYU pragma: keep

#include "ansi/colors.h"
#include "array/mod.h"
#include "format/u8.h"
#include "hash_table/mod.h"
#include "print/mod.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

__thread GSHL_HashTable GSHL_format_ht = {};
__thread GSHL_FormatSpecifiers GSHL_format_specifiers = {};

static int GSHL_compare_strings(const void *a, const void *b)
{
    return strcmp(*(const char **)a, *(const char **)b);
}

static usize GSHL_hash_idem(const GSHL_HashTableKey key) { return key.usize; }

void GSHL_format_specifiers_print(void)
{
    GSHL_DArrayTypeDecl(GSHL_FSS, const char *);
    GSHL_FSS fss = {};
    GSHL_DArray_init(&fss);

    GSHL_ARRAYN_FOREACH(GSHL_format_ht.table, GSHL_format_ht.table_size,
                        GSHL_HashTableEntry * entry)
    {
        for (const GSHL_HashTableEntry *current = entry; current != NULL;
             current = current->next) {
            const GSHL_FormatSpecifier *fs = current->value.opaque;
            GSHL_ARRAY_FOREACH(fs->specifiers, char *specifier)
            {
                if (specifier == NULL || specifier[0] == '\0') {
                    break;
                }

                // printf(GSHL_FG_CYAN("%s") "\n", specifier);
                GSHL_DArray_append(&fss, specifier);
            }
        }
    }

    qsort(fss.items, fss.count, sizeof(fss.items[0]), GSHL_compare_strings);
    const char *unique_fs = NULL;
    GSHL_ARRAYN_FOREACH(fss.items, fss.count, char *const fs)
    {
        if (unique_fs != NULL && strcmp(unique_fs, fs) == 0) {
            continue;
        }

        printf(GSHL_FG_CYAN("%s") "\n", fs);
        unique_fs = fs;
    }

    GSHL_DArray_destroy(&fss);
}

bool GSHL_format_specifiers_register(const GSHL_FormatSpecifier fs)
{

    if (GSHL_format_ht.table_size == 0) {
        GSHL_HashTable_init(&GSHL_format_ht, usize, GSHL_FormatSpecifier *,
                            GSHL_hash_idem);
    }

    GSHL_DArray_append(&GSHL_format_specifiers, fs);

    GSHL_ARRAY_FOREACH(fs.specifiers, char *specifier)
    {
        if (specifier == NULL || specifier[0] == '\0') {
            break;
        }

        const char *const start = specifier;
        const char *const end = specifier + strlen(specifier);
        const usize hash = GSHL_hash_format_specifier(start, end, NULL);
        if (GSHL_HashTable_search(&GSHL_format_ht, .usize = hash) != NULL) {
#ifdef GSHL_DEBUG
            GSHL_dprintln(STDERR_FILENO,
                          "Format Specifier already registered: %s", specifier);
#endif
            continue;
        }

        GSHL_ASSERT(fs.va_size % 4 == 0);

        GSHL_HashTable_insert(
            &GSHL_format_ht, .usize = hash,
            .opaque = &GSHL_format_specifiers
                           .items[GSHL_format_specifiers.count - 1]);
    }

    return true;
}

#define FS(BIT_SHIFT, ENUM, NAME, TYPE, OPTS, ...)                             \
    GSHL_FORMAT_SPECIFIER_REGISTER(                                            \
        NAME, .kind = GSHL_FORMAT_SPECIFIER_##ENUM,                            \
        .va_size = (((sizeof(TYPE) - 1) / 4) + 1) * 4,                         \
        .write = GSHL_write_##TYPE, .specifiers = {__VA_ARGS__})
GSHL_FORMAT_SPECIFIERS
#undef FS
