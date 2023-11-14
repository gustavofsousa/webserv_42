#include "./Webserv.hpp"

// Webserv::Webserv() {}

Webserv::Webserv(std::vector<Server> const& newServers)
 : servers(newServers) {}

Webserv::~Webserv() {}

static int printError(std::string const& error) {
	std::cout << "\033[1;31m" << error << "\033[0m" << std::endl;
	return (1);
}

void    Webserv::readDataClient(int i) {
	std::cout << "Reading data now." << std::endl;
	if (i < this->nbrServers) {
		conn.addClientSocket(this->servers[i].acceptCon());
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

void    Webserv::sendDataClient(int i) {
	std::cout << "I'm sending data back" << std::endl;
	send(this->conn.getFd(i).fd, "200", 4, 0);
}

int	Webserv::updateStatusPoll() {
	//std::cout << "Giving a poll" << std::endl;
	if (poll(this->conn.getPollFd().data(), this->conn.getPollFd().size(), -1) == -1)
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
		if (updateStatusPoll() == -1)
			return;
		for (size_t i = 0; i < this->conn.getPollFd().size(); i++)
		{
			if (ableToRead(i))
				this->readDataClient(i);
			else if (ableToWrite(i))
				this->sendDataClient(i);
			else if (pollError(i))
				printError("Error for poll revents");
			else
				std::cout  << "on [" << i << "] my revent is ->" << this->conn.getFd(i).revents << std::endl;
		}
	}
}

void    Webserv::setup(ParserServer configFile) {
	this->nbrServers = configFile.getNbrServers();
	std::cout << "Doing the setup of webserv" << std::endl;
}

bool	Webserv::ableToRead(int i) {
	return (this->conn.getFd(i).revents & POLLIN);
}
bool	Webserv::ableToWrite(int i) {
	return (this->conn.getFd(i).revents & POLLOUT);
}
bool	Webserv::pollError(int i) {
	return ((this->conn.getFd(i).revents & POLLERR)
		|| (this->conn.getFd(i).revents & POLLHUP)
		|| (this->conn.getFd(i).revents & POLLNVAL));
}
