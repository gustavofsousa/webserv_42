#include "ServerSocket.hpp"

void handleHTTPRequest(int client_socket) {
    // Read and parse the HTTP request from the client
    // Determine the HTTP method, URI, and other request details

    // Implement routing and handlers for different URIs and HTTP methods
    // Example: if (requested_uri == "/") serveHomePage(client_socket);
    // Example: if (requested_uri == "/submit" && http_method == "POST") handleFormSubmission(client_socket);

    // Generate an appropriate HTTP response and send it back to the client

    char buffer[1024];  // A buffer to store the incoming data
    ssize_t bytes_received;

    // Read the HTTP request from the client
    bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);

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


    //receive from client requested_uri
    // char buffer[1024];


    // if (requested_uri == "/jim") {
    //     std::cout << "Hey, This is Jim" << std::endl;
    // }
}

int main() {
    int port = 8080;

    ServerSocket server(port);
    server.Initialize();

    while (true) {
        int client_socket = server.Accept();

        // Handle the HTTP request from the client
        handleHTTPRequest(client_socket);

        close(client_socket); // Close the client socket after handling the request
    }

    server.Close();

    return 0;
}
