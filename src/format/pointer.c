#include "format/pointer.h"

#include <assert.h>
#include <string.h>

usize write_pointer(char *buf, const void *pointer,
                    const usize precomputed_count)
{
    static const char prefix[] = "0x";
    static const usize prefixLen = sizeof(prefix) - 1;

    if (buf == NULL || precomputed_count == 0) {
        if (pointer == NULL) {
            return sizeof("nil") - 1;
        }

        usize count = 0;
        uintptr_t address = (uintptr_t)pointer;
        do {
            address >>= 4;
            count += 1;
        } while (address != 0);

        return prefixLen + count; // two hex digits for each byte
    }

    if (pointer == NULL) {
        assert(precomputed_count == 3);
        memcpy(buf, "nil", 3);
        return precomputed_count;
    }

    uintptr_t address = (uintptr_t)pointer;

    // Fill the buffer with hexadecimal representation
    for (isize i = precomputed_count - prefixLen - 1; i >= 0 && address > 0;
         --i, address >>= 4) {
        buf[prefixLen + i] = "0123456789abcdef"[address % 16]; // Lowercase hex
    }

    memcpy(buf, prefix, prefixLen);

    return precomputed_count;
}
