#include "net/ip.h"

#include "format/register.h"
#include "format/write.h"
#include "types/mod.h"

#include <assert.h>

usize GSHL_write_Ipv4Addr(GSHL_FormatString *string,
                          const GSHL_FormatSpecifier *fs)
{
    const GSHL_Ipv4Addr *addr = fs->value.pointer;

    return GSHL_format_write(string, "{u16}.{u16}.{u16}.{u16}",
                             addr->octects[0], addr->octects[1],
                             addr->octects[2], addr->octects[3]);
}

GSHL_FORMAT_SPECIFIER_REGISTER(GSHL_Ipv4Addr,
                               .kind = GSHL_FORMAT_SPECIFIER_POINTER,
                               .va_size = sizeof(GSHL_Ipv4Addr *),
                               .write = GSHL_write_Ipv4Addr,
                               .specifiers = {"GSHL_Ipv4Addr", "Ipv4Addr"});
