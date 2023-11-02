#include "./Webserv.hpp"

// Webserv::Webserv() {}

Webserv::Webserv(Server const& newServer) : server(newServer) {}

Webserv::~Webserv() {}

// Im not saving new fd in poll_fd.
void    Webserv::readDataClient(int i) {
	if (i < num_servers) {
		std::cout << "Creating conection with a new client" << std::endl;
		// save socket in struct pollfd
		pollfd pfd;
		pfd.fd = this->server.acceptCon();
		pfd.events = POLLIN | POLLOUT;
		this->poolAllFd.push_back(pfd);
		return;
	}

	int	sock_fd_client = this->poolAllFd[i].fd;
	char buffer[1024];
	ssize_t bytes_received;
	bytes_received = recv(sock_fd_client, buffer, sizeof(buffer), 0);

	if (bytes_received < 0) {
		std::cerr << "Error reading request from client." << std::endl;
		return;
	}
	if (bytes_received == 0) {
		std::cerr << "Client closed the connection." << std::endl;
		this->poolAllFd.erase(this->poolAllFd.begin() + i);
		return;
	}
	std::cout << "Received " << bytes_received << " bytes from client." << std::endl;
	std::cout << "Message: " << buffer << std::endl;
	buffer[bytes_received] = '\0';
	
	// TO-DO
	// HTTPRequest  client_request = parseRequest(buffer);
	// HTTPResponse response = process_request(client_reqest); // Here execute methods or CGI
}

void    Webserv::sendDataClient() {
	
	// TO-DO
	// send(this->response);
	std::cout << "I'm sending data back" << std::endl;
}

static int	updateStatusPoll(std::vector<pollfd> poolAllFd) {
	std::cout << "Giving a poll" << std::endl;
	if (poll(poolAllFd.data(), poolAllFd.size(), -1) == -1)
	{
		std::cerr << "Error in webserv.cpp: " << strerror(errno) << std::endl;
		return (-1);
	}
	return (0);
}

void	Webserv::addNewSocket(int socket_fd) {
	pollfd pfd;
	pfd.fd = socket_fd;
	pfd.events = POLLIN | POLLOUT;
	this->poolAllFd.push_back(pfd);

}

void	Webserv::addServersSockets() {
	std::cout << "Adding servers sockets" << std::endl;
	// for (size_t i = 0; i < servers.size(); i++)
	// {
		// addNewSocket(servers[i]._fd_socket);
	// }
	addNewSocket(this->server._fd_socket);
}

void    Webserv::start() {

	if (updateStatusPoll(this->poolAllFd) == -1)
		return;
	// Inserting socket server to monitorate.
	addServersSockets();

	while (42) {


		for (size_t i = 0; i < this->poolAllFd.size(); i++)
		{
			std::cout << "client number: " << i << std::endl;
			if (is_available_to_read(i))
				this->readDataClient(i);
			else if (is_available_to_write(i))
				this->sendDataClient();
			// else
				// std::cerr << "error in receive poll" << std::endl;
		}
	}
}

void    Webserv::setup() {
	this->num_servers = 1;
	std::cout << "Doing the setup with config" << std::endl;
}

bool	Webserv::is_available_to_read(int client) {
	return (this->poolAllFd[client].revents & POLLIN);
}
bool	Webserv::is_available_to_write(int client) {
	return (this->poolAllFd[client].revents & POLLOUT);
}
