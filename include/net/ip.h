// gshl-priority: 70
#ifndef INCLUDE_NET_IP_H_
#define INCLUDE_NET_IP_H_

#include "format/register.h"
#include "types/mod.h"

/// {{{ Types

typedef union GSHL_Ipv4Addr {
    u8 octects[4];
    u32 value;
} GSHL_Ipv4Addr;

/// }}}

/// {{{ Functions

GSHL_FormatSpecifierWriteFunction GSHL_write_Ipv4Addr;

/// }}}

#ifdef GSHL_STRIP_PREFIX
typedef GSHL_Ipv4Addr Ipv4Addr;
#endif

#endif // INCLUDE_NET_IP_H_
