#define GSHL_STRIP_PREFIX
#define GSHL_IMPLEMENTATION
#include "../../gshl.h"

int main()
{
    SocketAddrV4 server_socket = {.address.octects = {127, 0, 0, 1},
                                  .port = 8080};
    println("server_socket = {SocketAddrV4}", &server_socket);

    if (!SocketAddrV4_socket(&server_socket)) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    if (!SocketAddrV4_bind(&server_socket)) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (!SocketAddrV4_listen(&server_socket, 3)) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    // TODO: Accept multiple connections
    SocketAddrV4 client_socket = SocketAddrV4_accept(&server_socket);
    if (client_socket.fd < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }
    println("client_socket = {SocketAddrV4}", &client_socket);

    // Reading and echoing back data
#define BUFFER_SIZE 4096
    char buffer[BUFFER_SIZE] = {0};
    while (1) {
        buffer[0] = '\0';

        const i32 valread = read(client_socket.fd, buffer, BUFFER_SIZE);
        if (valread <= 0) {
            break;
        }

        char *const new_line = strchr(buffer, '\n');
        if (new_line == NULL) {
            buffer[valread] = '\0';
        }
        else {
            *new_line = '\0';
        }
        println("Received: %s", buffer);
        if (new_line != NULL) {
            *new_line = '\n';
        }

        send(client_socket.fd, buffer, valread, 0);
    }

    if (!SocketAddrV4_close(&client_socket)) {
        dprintln(STDERR_FILENO, "Failed to close client socket");
    }
    if (!SocketAddrV4_close(&server_socket)) {
        dprintln(STDERR_FILENO, "Failed to close server socket");
    }

    return 0;
}
