#ifndef INCLUDE_NET_SOCKET_ADDR_H_
#define INCLUDE_NET_SOCKET_ADDR_H_

#include "format/mod.h"
#include "types/mod.h"

#include "ip.h"

/// {{{ Types

typedef struct GSHL_SocketAddrV4 {
    GSHL_Ipv4Addr address;
    u16 port;
    Fd fd;
} GSHL_SocketAddrV4;

/// }}}

/// {{{ Functions

GSHL_FormatSpecifierWriteFunction GSHL_write_SocketAddrV4;
bool GSHL_SocketAddrV4_socket(GSHL_SocketAddrV4 *sock);
bool GSHL_SocketAddrV4_bind(const GSHL_SocketAddrV4 *sock);
bool GSHL_SocketAddrV4_listen(const GSHL_SocketAddrV4 *sock,
                              const usize queue_size);
GSHL_SocketAddrV4
GSHL_SocketAddrV4_accept(const GSHL_SocketAddrV4 *server_socket);
bool GSHL_SocketAddrV4_connect(const GSHL_SocketAddrV4 *server_socket,
                               GSHL_SocketAddrV4 *const client_socket);
bool GSHL_SocketAddrV4_close(GSHL_SocketAddrV4 *sock);

/// }}}

#ifdef GSHL_STRIP_PREFIX
typedef GSHL_SocketAddrV4 SocketAddrV4;
#    define SocketAddrV4_socket GSHL_SocketAddrV4_socket
#    define SocketAddrV4_bind GSHL_SocketAddrV4_bind
#    define SocketAddrV4_listen GSHL_SocketAddrV4_listen
#    define SocketAddrV4_accept GSHL_SocketAddrV4_accept
#    define SocketAddrV4_connect GSHL_SocketAddrV4_connect
#    define SocketAddrV4_close GSHL_SocketAddrV4_close
#endif

#endif // INCLUDE_NET_SOCKET_ADDR_H_
