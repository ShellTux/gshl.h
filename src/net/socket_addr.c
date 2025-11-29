#include "net/socket_addr.h"

#include "format/mod.h"
#include "macros/mod.h"
#include "types/mod.h"

#include <arpa/inet.h>
#include <assert.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

extern GSHL_FormatSpecifiers GSHL_additional_format_specifiers;

usize GSHL_write_SocketAddrV4(GSHL_FormatString *string,
                              const GSHL_FormatSpecifier *fs)
{
    const GSHL_SocketAddrV4 *socket = fs->value.pointer;

    return GSHL_format_write(string,
                             "SocketAddrV4 {{\n"
                             "  .address = {Ipv4Addr}\n"
                             "  .port = {u16}\n"
                             "}}",
                             &socket->address, socket->port);
}

bool GSHL_SocketAddrV4_socket(GSHL_SocketAddrV4 *sock)
{
    sock->fd = socket(AF_INET, SOCK_STREAM, 0);
    return sock->fd != 0;
}

bool GSHL_SocketAddrV4_bind(const GSHL_SocketAddrV4 *sock)
{
    struct sockaddr_in address;

    // Setting up the address structure
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = sock->address.value;
    address.sin_port = htons(sock->port);

    // Binding socket to the specified port
    return bind(sock->fd, (struct sockaddr *)&address, sizeof(address)) >= 0;
}

bool GSHL_SocketAddrV4_listen(const GSHL_SocketAddrV4 *sock,
                              const usize queue_size)
{
    return listen(sock->fd, (int)queue_size) >= 0;
}

GSHL_SocketAddrV4 GSHL_SocketAddrV4_accept(const GSHL_SocketAddrV4 *sock)
{
    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);

    const Fd fd = accept(sock->fd, (struct sockaddr *)&client_address,
                         &client_address_len);
    return (GSHL_SocketAddrV4){
        .address.value = client_address.sin_addr.s_addr,
        .port = ntohs(client_address.sin_port),
        .fd = fd,
    };
}

bool GSHL_SocketAddrV4_connect(const GSHL_SocketAddrV4 *server_socket,
                               GSHL_SocketAddrV4 *client_socket)
{
    client_socket->fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket->fd < 0) {
        return false;
    }

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(server_socket->port);
    serv_addr.sin_addr.s_addr = server_socket->address.value;

    if (connect(client_socket->fd, (struct sockaddr *)&serv_addr,
                sizeof(serv_addr)) < 0) {
        return false;
    }

    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    getsockname(client_socket->fd, (struct sockaddr *)&client_addr, &addr_len);

    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);

    client_socket->address.value = client_addr.sin_addr.s_addr;
    client_socket->port = ntohs(client_addr.sin_port);

    return true;
}

bool GSHL_SocketAddrV4_close(GSHL_SocketAddrV4 *sock)
{
    if (sock->fd == 0) {
        return false;
    }
    else {
        close(sock->fd);
        sock->fd = 0;
        return true;
    }
}

GSHL_FORMAT_SPECIFIER_REGISTER_ADDITIONAL(
    GSHL_SocketAddrV4, .kind = GSHL_FORMAT_SPECIFIER_POINTER,
    .va_size = sizeof(GSHL_SocketAddrV4 *), .write = GSHL_write_SocketAddrV4,
    .specifiers = {"GSHL_SocketAddrV4", "SocketAddrV4"});
