#define GSHL_IMPLEMENTATION
#define GSHL_STRIP_PREFIX
#include "../../gshl.h"

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main()
{
    SocketAddrV4 server_socket = {.address.octects = {127, 0, 0, 1},
                                  .port = 8080};
    SocketAddrV4 client_socket = {};

    if (!SocketAddrV4_connect(&server_socket, &client_socket)) {
        dprintln(STDERR_FILENO, "Failed to connect to {SocketAddrV4}",
                 &server_socket);
    }
    println("client_socket = {SocketAddrV4}", &client_socket);

    const char *const message = "Hello from client";
    char buffer[BUFFER_SIZE] = {0};

    // Sending a message to the server
    send(client_socket.fd, message, strlen(message), 0);
    println("Message sent");

    // Reading the response from the server
    const i32 valread = read(client_socket.fd, buffer, BUFFER_SIZE);
    buffer[valread] = '\0';
    println("Response from server: %s", buffer);

    if (!SocketAddrV4_close(&client_socket)) {
        dprintln(STDERR_FILENO, "Failed to close client socket");
    }

    return 0;
}
