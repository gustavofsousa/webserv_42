#include "./Webserv.hpp"

Webserv::Webserv(std::vector<Server> const& newServers, ParserServer const& configFile)
 : servers(newServers) {
	this->_nbrServers = configFile.getNbrServers();
/*
        int i = 0;
        size_t j;
        size_t k;
        while (i < this->_nbrServers)
        {
            j = 0;
            while (j < this->servers[i].getServerConf().getLocation().size())
            {
                k = 0;
                while ( k < this->servers[i].getServerConf().getLocation()[j].getIndex().size())
                {
                    std::cout << "in server " << i << " Location: " << j << " index para k: " << k << " é: " << this->servers[i].getServerConf().getLocation()[j].getIndex()[k] << std::endl;
                    k++;
                }
                j++;
            }
//            std::cout << "in server " << (i + 1) << " Port: " << servers[i].getServerConf().getPort() << std::endl;
            i++;
        }
        exit(1);
*/
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

//	Request newRequest(newClient);
	Request newRequest(newClient, this->servers[i].getServerConf());
	this->_requests.push_back(newRequest);
	return true;
}

bool    Webserv::readDataClient(int i)
{
	int clientWithMessage;
	int indexRequest;

	try {
		indexRequest = i - this->_nbrServers;
		if (this->isRequestFromServer(i))
			return (this->openNewConnection(i));
// std::cout << "At read: is ready? " << this->_requests[indexRequest].isReady() << std::endl;
		if (this->_requests[indexRequest].isReady())
			return true;
		clientWithMessage = this->conn.getFd(i).fd;
		if (this->_requests[indexRequest].receiveFromClient(clientWithMessage) == false)
		{
			this->conn.closeConnection(i);
			this->_requests[indexRequest].reset();
			this->_requests.erase(this->_requests.begin() + indexRequest);
			return false;
		}
		this->_requests[indexRequest].parseRequest();
//		this->_requests[indexRequest].fileRequested();
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
		indexRequest = i - this->_nbrServers;
// std::cout << "At send: is it ready? " << this->_requests[indexRequest].isReady() << std::endl;
		if (this->_requests[indexRequest].isReady())
		{
			Client		client(this->_requests[indexRequest], response);
			// std::cout << "####### RESPONSE ######" << std::endl << response.httpMessage << std::endl;
		    send(this->conn.getFd(i).fd, response.httpMessage.c_str(),
				response.httpMessage.size(), MSG_NOSIGNAL);
            // if (bytes != this->_response.totalLength())
            //     std::cerr << "No send adequade number of bytes" << std::endl;
            this->conn.closeConnection(i);
			this->_requests[indexRequest].reset();
			this->_requests.erase(this->_requests.begin() + indexRequest);
            std::cout << "I deleted the request after send it. There is " << this->_requests.size() << " requests now" << std::endl;
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
				if (this->ableToRead(i))
					this->readDataClient(i);
				else if (this->ableToWrite(i))
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
