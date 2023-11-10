#include "./Webserv.hpp"

// Webserv::Webserv() {}

Webserv::Webserv(std::vector<Server> const& newServers)
 : servers(newServers) {}

Webserv::~Webserv() {}

static void printError(std::string const& error) {
	std::cout << "\033[1;31m" << error << "\033[0m" << std::endl;
}

void    Webserv::readDataClient(int i) {
	if (i < this->nbrServers) {
		std::cout << "Creating conection with a new client" << std::endl;
		conn.addNewSocket(this->servers[i].acceptCon());
		return;
	}

	int		sock_fd_client = this->conn.getFd(i).fd;
	char	buffer[1024];
	ssize_t bytes_received;
	bytes_received = recv(sock_fd_client, buffer, sizeof(buffer), 0);

	if (bytes_received == -1) {
		printError("Error reading request from client.");
		return;
	} else if (bytes_received == 0) {
		conn.closeConnection(i);
		return;
	}
	std::cout << "Received " << bytes_received << " bytes from client." << std::endl;
	std::cout << "Message: " << buffer << std::endl;
	buffer[bytes_received] = '\0';

	// Response		response;
	// Request		request(buffer);
	// Client		client(request, response);
	// Response		response(client);
	// Here execute	methods or CGI
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
		printError("Error in poll: ");
		printError(strerror(errno));
		return (-1);
	}
	return (0);
}

void    Webserv::start() {
	// Inserting the sockets of servers to monitorate.
	conn.addServersSockets(this->servers);

	while (42)
	{
		if (updateStatusPoll(this->conn.getPollFd()) == -1)
			return;
		for (size_t i = 0; i < this->conn.getPollFd().size(); i++)
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

void    Webserv::setup(ParserServer configFile) {
	this->nbrServers = configFile.getNbrServers();
	std::cout << "Doing the setup with config" << std::endl;
}

bool	Webserv::ableToRead(int i) {
	return (this->conn.getFd(i).revents & POLLIN);
}
bool	Webserv::ableToWrite(int i) {
	return (this->conn.getFd(i).revents & POLLOUT);
}

