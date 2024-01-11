#include "./Server.hpp"

Server::Server() {}
Server::~Server() {}

Server::Server(int port) :_fd_socket(-1), _port(port) {}

void Server::initialize() {
	this->_fd_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_fd_socket < 0) {
		perror("Error creating socket.");
		exit(1);
	}
	// Bind the socket to a specific IP address and port
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(this->_port); //Reorganize order of bytes to network order.
	server_addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(this->_fd_socket, reinterpret_cast<struct sockaddr*>(&server_addr), sizeof(server_addr)) < 0) { // choose a port to itself.
		perror("Error binding socket");
		exit(1);
	}
	// Listen for incoming connections
	if (listen(this->_fd_socket, 5) < 0) {
		perror("Error listening for connections.");
		exit(1);
	}
}

struct clientInfo 	Server::acceptCon() const {
	// Accept incoming connections and get a file descriptor for reading and writing
	struct clientInfo	clientInfo;

	clientInfo.addressLen = sizeof(clientInfo.address);
	clientInfo.socket = accept(this->_fd_socket,
			reinterpret_cast<struct sockaddr*>(&clientInfo.address),
		&clientInfo.addressLen);

	clientInfo.port = ntohs(clientInfo.address.sin_port);
	// Use if needs to convert ip to string
	// client.ipString = inet_ntoa(clientInfo.address.sin_addr);
  	clientInfo.ip = ntohl(clientInfo.address.sin_addr.s_addr);
	return clientInfo;
}

void Server::closeCon() {
	if (this->_fd_socket >= 0) {
		close(this->_fd_socket);
	}
}

void    Server::setServerConf(const ConfigFile & server)
{
	this->_serverConf = server;
}

const ConfigFile &   Server::getServerConf(void)
{
	return (this->_serverConf);
}