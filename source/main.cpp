#include "../include/Webserv.hpp"

void handleHTTPRequest(int sock_fd_client) {
    char buffer[1024];  // A buffer to store the incoming data
    ssize_t bytes_received;

    // Read the HTTP request from the client
    bytes_received = recv(sock_fd_client, buffer, sizeof(buffer), 0);

    if (bytes_received < 0) {
        std::cerr << "Error reading request from client." << std::endl;
        return;
    }

    if (bytes_received == 0) {
        std::cerr << "Client closed the connection." << std::endl;
        return;
    }
    std::cout << "Received " << bytes_received << " bytes from client." << std::endl;
    std::cout << "Message: " << buffer << std::endl;

    // Null-terminate the received data to make it a C-string
    buffer[bytes_received] = '\0';
}

int main() {
    int port = 8080;

    Server server(port);
    server.initialize();

    Webserv webserv(server);
    webserv.setup();
    webserv.start();
    // while (true) {
    //     int sock_fd_client = server.acceptCon();

    //     handleHTTPRequest(sock_fd_client);

    //     close(sock_fd_client); // Close the client socket after handling the request
    // }

    server.closeCon();

    return 0;
}
