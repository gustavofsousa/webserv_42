#include "./Webserv.hpp"

Webserv::Webserv(std::vector<Server> const& newServers, ParserServer const& configFile)
 : servers(newServers) {
	this->_nbrServers = configFile.getNbrServers();
 }

Webserv::~Webserv() {}

static void printError(std::string const& error) {
	std::cout << "\033[1;31m" << error << "\033[0m" << std::endl;
}

bool		Webserv::openNewConnection(int i)
{
	int newClient;
	newClient = this->servers[i].acceptCon();
	this->conn.addClientSocket(newClient);

	Request newRequest(newClient);
	this->_requests.push_back(newRequest);
	return true;
}

bool    Webserv::readDataClient(int i)
{
	int clientWithMessage;
	int indexRequest;

	try {
		if (this->isRequestFromServer(i))
			return (this->openNewConnection(i));
		clientWithMessage = this->conn.getFd(i).fd;
		indexRequest = i - this->_nbrServers;
		if (this->_requests[indexRequest].receiveFromClient(clientWithMessage) == false)
		{
			this->conn.closeConnection(i);
			this->_requests[indexRequest].reset();
			this->_requests.erase(this->_requests.begin() + indexRequest);
			return false;
		}
		this->_requests[indexRequest].parseRequest();
	}
	catch (std::exception & e) {
		std::cout << "Error in readDataClient: " << e.what() << std::endl;
		return false;
	}
	return true;
}

// Check if size of response is greater than permited.
bool    Webserv::sendDataClient(int i) {
	Response	response;
	int 		indexRequest;

	try {
std::cout << "I arrived at sendDataClient" << std::endl;
		indexRequest = i - this->_nbrServers;
		if (this->_requests[indexRequest].isReady())
		{
			Client		client(this->_requests[indexRequest], response);
			// std::cout << "Sending data of client: " << i << std::endl;
			// std::cout << "####### RESPONSE ######" << std::endl << this->_response.httpMessage << std::endl;
			send(this->conn.getFd(i).fd, response.httpMessage.c_str(),
				response.httpMessage.size(), 0);
			this->_requests[indexRequest].reset();
		}
	}
	catch (std::exception & e) {
		std::cout << "Error in sendDataClient: " << e.what() << std::endl;
		return false;
	}
	return true;
}

int	Webserv::updateStatusPoll()
{
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
		std::cerr << e.what() << std::endl;
	}
	return (-1);
}

void    Webserv::start()
{
	try {
		// Inserting the sockets of servers to monitorate.
		conn.addServersSockets(this->servers);
		while (Utils::_serverRunning)
		{
			if (updateStatusPoll() == -1)
				return;
			for (size_t i = 0; i < this->conn.getPollFd().size(); i++)
			{
				std::cout << this->conn.getFd(i).revents << std::endl;
				//isMoreThanReady(i);
				if (this->ableToRead(i))
					this->readDataClient(i);
				if (this->ableToWrite(i))
					this->sendDataClient(i);
				else if (this->pollError(i))
					printError("Error for poll revents");
			}
		}
	}
	catch (std::exception & e) {
		std::cout << "Error in start: " << e.what() << std::endl;
	}
}

bool	Webserv::isMoreThanReady(int i) {
	int indexRequest;

	indexRequest = i - this->_nbrServers;
	if (indexRequest < 0 || this->ableToWrite(i) == false
		|| this->_requests[indexRequest].isReady() == false)
		return false;
	std::cout << "I'm sending by a shortcut" << std::endl;
	this->sendDataClient(i);
	return true;
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
