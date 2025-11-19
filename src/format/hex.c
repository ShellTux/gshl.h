#include "format/hex.h"

#include <string.h>

usize write_hex(char *buf, const usize hex, const usize precomputed_count)
{
    static const char prefix[] = "0x";
    static const usize prefixLen = sizeof(prefix) - 1;

    if (buf == NULL || precomputed_count == 0) {
        usize count = 0;
        usize remaining = hex;
        do {
            remaining >>= 4;
            count += 1;
        } while (remaining != 0);

        return prefixLen + count;
    }

    usize remaining = hex;
    for (isize i = precomputed_count - prefixLen - 1; i >= 0; --i) {
        buf[prefixLen + i] = "0123456789ABCDEF"[remaining % 16];
        remaining >>= 4;
    }

    memcpy(buf, prefix, prefixLen);

    return precomputed_count;
}
