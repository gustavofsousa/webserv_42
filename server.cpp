#include "ServerSocket.hpp"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

ServerSocket::ServerSocket(int port) : port(port), server_socket(-1) {}

void ServerSocket::Initialize() {
    // Create a socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        std::cerr << "Error creating socket." << std::endl;
        exit(1);
    }

    // Bind the socket to a specific IP address and port
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, reinterpret_cast<struct sockaddr*>(&server_address), sizeof(server_address) < 0)) {
        std::cerr << "Error binding socket." << std::endl;
        exit(1);
    }

    // Listen for incoming connections
    if (listen(server_socket, 5) < 0) {
        std::cerr << "Error listening for connections." << std::endl;
        exit(1);
    }
}

int ServerSocket::Accept() {
    // Accept incoming connections and get a file descriptor for reading and writing
    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);
    int client_socket = accept(server_socket, reinterpret_cast<struct sockaddr*>(&client_address), &client_address_len);

    return client_socket;
}

void ServerSocket::Close() {
    if (server_socket >= 0) {
        close(server_socket);
    }
}
