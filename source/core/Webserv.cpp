#include "./Webserv.hpp"

Webserv::Webserv(std::vector<Server> const& newServers, ParserServer const& configFile)
 : servers(newServers) {
	this->_nbrServers = configFile.getNbrServers();
 }

Webserv::~Webserv() {}

static void printError(std::string const& error) {
	std::cout << "\033[1;31m" << error << "\033[0m" << std::endl;
}

void    Webserv::readDataClient(int i)
{
	try {
		if (this->isRequestFromServer(i)) {
			int newClient;
			newClient = this->servers[i].acceptCon();
			Request newRequest(newClient);
			this->conn.addClientSocket(newClient);
			this->_requests.push_back(newRequest);
			return;
		}
		int clientWithMessage = this->conn.getFd(i).fd;
		int indexRequest = i - this->_nbrServers;

		if (this->_requests[indexRequest].receiveFromClient(clientWithMessage))
			this->_requests[indexRequest].parseRequest();
		else
		{
			this->conn.closeConnection(i);
			this->_requests[indexRequest].reset();
			this->_requests.erase(this->_requests.begin() + i);
		}
	}
	catch (std::exception & e) {
		std::cout << "Error in readDataClient: " << e.what() << std::endl;
	}
}

void    Webserv::sendDataClient(int i) {
	Response	response;
	try {
		if (this->_requests[i].isReady())
		{
		std::cout << "I want to send data" << std::endl;
			Client		client(this->_requests[i], response);
			// Check if size of response is greater than permited.
			std::cout << "Sending data of client: " << i << std::endl;
			// std::cout << "####### RESPONSE ######" << std::endl << this->_response.httpMessage << std::endl;
			send(this->conn.getFd(i).fd, response.httpMessage.c_str(),
				response.httpMessage.size(), 0);
			this->_requests[i].reset();
		}
	}
	catch (std::exception & e) {
		std::cout << "Error in sendDataClient: " << e.what() << std::endl;
	}
}

int	Webserv::updateStatusPoll() {
	// std::cout << "Giving a poll" << std::endl;
	try
	{
		if (poll(this->conn.getPollFd().data(), this->conn.getPollFd().size(), -1) == -1)
		{
			printError("Error in poll: ");
			printError(strerror(errno));
			return (-1);
		}
		return (0);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	return (-1);
}

void    Webserv::start() {
	try {
	// Inserting the sockets of servers to monitorate.
	conn.addServersSockets(this->servers);
	while (42)
	{
		if (updateStatusPoll() == -1)
			return;
		for (size_t i = 0; i < this->conn.getPollFd().size(); i++)
		{
			// std::cout << "Tamanho do vector -> " << this->conn.getPollFd().size() << std::endl;
			if (ableToRead(i))
				this->readDataClient(i);
			else if (ableToWrite(i)){
				this->sendDataClient(i);
			}
			else if (pollError(i))
				printError("Error for poll revents");
			// else
				// std::cout  << "on [" << i << "] my revent is ->" << this->conn.getFd(i).revents << std::endl;
		}
	}
	}
	catch (std::exception & e) {
		std::cout << "Error in start: " << e.what() << std::endl;
	}
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

bool	Webserv::isRequestFromServer(int i) {
	return (i < this->_nbrServers);
}
