#include "./Webserv.hpp"

Webserv::Webserv(std::vector<Server> const& newServers)
 : servers(newServers) {}

Webserv::~Webserv() {}

static void printError(std::string const& error) {
	std::cout << "\033[1;31m" << error << "\033[0m" << std::endl;
}

void    Webserv::readDataClient(int i) {
	if (this->isRequestFromServer(i)) {
        int newClient = this->servers[i].acceptCon();
	    this->conn.addClientSocket(newClient);
        this->_requests.push_back(Request(newClient));
        return;
    }
    int clientWithMessage = this->conn.getFd(i).fd;
    int indexRequest = i - this->_nbrServers;

	if (this->_requests[indexRequest].receiveFromClient(clientWithMessage))
        this->_requests[indexRequest].parseRequest();
    else
    {
        this->conn.closeConnection(i);
        // this->_requests[i].clearAll();
        this->_requests.erase(this->_requests.begin() + i);
    }
}

void    Webserv::sendDataClient(int i) {
    if (this->_requests[i].isReady())
    {
        Client		client(this->_requests[i], this->_response);
        // Check if size of response is greater than permited.
        if (this->_response.httpMessage.empty())
            return;
        std::cout << "Sending data to client" << std::endl;
        // std::cout << "####### RESPONSE ######" << std::endl << this->_response.httpMessage << std::endl;
        send(this->conn.getFd(i).fd,
            this->_response.httpMessage.c_str(),
            this->_response.httpMessage.size(), 0);
        this->_response.httpMessage.clear();
        // this->_requests[i].clearAll();
        this->_requests[i].setReadyFalse();
    }
}

int	Webserv::updateStatusPoll() {
	// std::cout << "Giving a poll" << std::endl;
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

void    Webserv::setup(ParserServer configFile) {
	this->_nbrServers = configFile.getNbrServers();
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

bool	Webserv::isRequestFromServer(int i) {
	return (i < this->_nbrServers);
}
