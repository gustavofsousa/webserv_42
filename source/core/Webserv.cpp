#include "./Webserv.hpp"

// Webserv::Webserv() {}

Webserv::Webserv(std::vector<Server> const& newServers)
 : servers(newServers) {}

Webserv::~Webserv() {}

void	Webserv::closeConnection(int index) {
	std::cerr << "Closing the connection: " << index << std::endl;
	close(this->poolAllFd[index].fd);
	this->poolAllFd.erase(this->poolAllFd.begin() + index);
}

// Im not saving new fd in poll_fd.
void    Webserv::readDataClient(int i) {
	if (i < num_servers) {
		std::cout << "Creating conection with a new client" << std::endl;
		int	fd_client = this->servers[i].acceptCon();
		addNewSocket(fd_client);
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
		closeConnection(i);
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
	for (size_t i = 0; i < servers.size(); i++)
	{
		addNewSocket(servers[i]._fd_socket);
	}
}

void    Webserv::start() {
	// Inserting socket server to monitorate.
	addServersSockets();

	while (42)
	{
		if (updateStatusPoll(this->poolAllFd) == -1)
			return;
		for (size_t i = 0; i < this->poolAllFd.size(); i++)
		{
			std::cout << "client number: " << i << std::endl;
			if (ableToRead(i))
				this->readDataClient(i);
			else if (ableToWrite(i))
				this->sendDataClient();
			// maybe some checkPollError() here.
		}
	}
}

void    Webserv::setup() {
	this->num_servers = 1;
	std::cout << "Doing the setup with config" << std::endl;
}

bool	Webserv::ableToRead(int client) {
	return (this->poolAllFd[client].revents & POLLIN);
}
bool	Webserv::ableToWrite(int client) {
	return (this->poolAllFd[client].revents & POLLOUT);
}
