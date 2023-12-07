#include "./Webserv.hpp"

Webserv::Webserv(std::vector<Server> const& newServers, ParserServer const& configFile)
 : servers(newServers) {
	this->_nbrServers = configFile.getNbrServers();
 
 /*
	std::cout << "construtor da webserver" << std::endl;
	int i;
	i = 0;
	while (i < this->_nbrServers)
	{
		std::cout << "in server " << (i + 1) << " Port: " << this->servers[i].getServerConf().getPort() << std::endl;
		i++;
	}
//	exit(1);
*/
 }

Webserv::~Webserv() {}

static void printError(std::string const& error) {
	std::cout << "\033[1;31m" << error << "\033[0m" << std::endl;
}

bool		Webserv::deleteConnection(int i, int indReq)
{
	this->conn.closeConnection(i);
	this->_requests[indReq].reset();
	this->_requests.erase(this->_requests.begin() + indReq);
	std::cout << "I deleted the request after send it. There is " << this->_requests.size() << " requests now" << std::endl;
	return true;
}

bool		Webserv::openNewConnection(int i)
{
	int newClient;
	newClient = this->servers[i].acceptCon();
	this->conn.addClientSocket(newClient);

//	Request newRequest(newClient);
	Request newRequest(newClient, this->servers[i].getServerConf());
	this->_requests.push_back(newRequest);
	return true;
}

bool    Webserv::readDataClient(int i, int indReq)
{
	int clientWithMessage;

	try {
		if (this->isRequestFromServer(i))
			return (this->openNewConnection(i));
		if (this->_requests[indReq].isReady())
			return true;
		clientWithMessage = this->conn.getFd(i).fd;
		if (this->_requests[indReq].receiveFromClient(clientWithMessage) == false)
		{
			this->deleteConnection(i, indReq);
			return false;
		}
		this->_requests[indReq].parseRequest();
	}
	catch (std::exception & e) {
		std::cout << "Error in readDataClient: " << e.what() << std::endl;
		return false;
	}
	return true;
}

// Check if size of response is greater than permited.
bool    Webserv::sendDataClient(int i, int indReq) {
	Response	response;

	try {
		if (this->_requests[indReq].isReady())
		{
			Client		client(this->_requests[indReq], response);
			// std::cout << "####### RESPONSE ######" << std::endl << response.httpMessage << std::endl;
		    send(this->conn.getFd(i).fd, response.httpMessage.c_str(),
				response.httpMessage.size(), MSG_NOSIGNAL);
            // if (bytes != this->_response.totalLength())
            //     std::cerr << "No send adequade number of bytes" << std::endl;
			this->deleteConnection(i, indReq);
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
	int 		indReq;

	try {
		// Inserting the sockets of servers to monitorate.
		conn.addServersSockets(this->servers);
		while (Utils::_serverRunning)
		{
			if (updateStatusPoll() == -1)
				return;
			for (size_t indServ = 0; indServ < this->conn.getPollFd().size(); indServ++)
			{
				indReq = indServ - this->_nbrServers;
				if (this->ableToRead(indServ))
					this->readDataClient(indServ, indReq);
				else if (this->ableToWrite(indServ))
					this->sendDataClient(indServ, indReq);
				else if (this->pollError(indServ))
					printError("Error for poll revents");
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
