#include "ServerSocket.hpp"


ServerSocket::ServerSocket(int port) : port(port), fd_socket(-1) {}

void ServerSocket::initialize() {
	// Create a socket
	fd_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (fd_socket < 0) {
		perror("Error creating socket.");
		exit(1);
	}

	// Bind the socket to a specific IP address and port
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port); //Reorganize order of bytes to network order.
	server_addr.sin_addr.s_addr = INADDR_ANY; 

	if (bind(fd_socket, reinterpret_cast<struct sockaddr*>(&server_addr), sizeof(server_addr)) < 0) { // choose a port to itself.
		perror("Error binding socket");
		exit(1);
	}

	// Listen for incoming connections
	if (listen(fd_socket, 5) < 0) {
		perror("Error listening for connections.");
		exit(1);
	}
}

int ServerSocket::acceptCon() {
	// Accept incoming connections and get a file descriptor for reading and writing
	struct sockaddr_in client_address;
	socklen_t client_address_len = sizeof(client_address);
	int client_socket = accept(fd_socket, reinterpret_cast<struct sockaddr*>(&client_address), &client_address_len);

	return client_socket;
}

void ServerSocket::close() {
	if (fd_socket >= 0) {
		close(fd_socket);
	}
}
