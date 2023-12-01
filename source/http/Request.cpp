#include "Request.hpp"

/*******************************************************/
/*						Constructors.		           */
/*******************************************************/
Request::Request(void) {
	this->_delimeter = "\r\n\r\n";
    this->_ready = false;
}

Request::Request(int newClient) {
    this->_fromClient = newClient;
	this->_delimeter = "\r\n\r\n";
    this->_ready = false;
	this->_contentLength = 0;
}

Request::Request(int newClient, ConfigFile _configFile) {
    this->_fromClient = newClient;
	this->_delimeter = "\r\n\r\n";
    this->_ready = false;
	this->_contentLength = 0;
	this->_serverConf = _configFile;

/*
	std::cout << "construtor da Request port: " << this->_serverConf.getPort() << " path: " << this->_serverConf.getRoot() << " getIndex: " << this->_serverConf.getIndex()[0] << std::endl;
	std::cout << "tamanho do location: " << this->_serverConf.getLocation().size() << std::endl;
	size_t	i = 0;
	while (i < this->_serverConf.getLocation().size())
	{
		std::cout << "Location path: " << this->_serverConf.getLocation()[i].getPath() << std::endl;
		i++;
	}
*/
}

Request::~Request(void) {}

static void printYellow(std::string const& str) {
	std::cout << "\033[1;33m" << str << "\033[0m" << std::endl;
}

// TIRAR ERRNO DAQUI
int	Request::checkBytesReceived(ssize_t bytes_received)
{
	if (bytes_received == -1)
	{
		std::cerr << "Error in recv: " << strerror(errno) << std::endl;
		return (-1);
	}
	else if (bytes_received == 0)
	{
		std::cout << "Client disconnected" << std::endl;
		return (0);
	}
	return (1);
}


int		Request::getHeader(int client) {
	char		buffer[BUFFER_SIZE];
	int			bytes;
	size_t		pos;

    if (this->_header.empty() == false) {
        return (0);
	}
	bytes = recv(client, buffer, BUFFER_SIZE - 1, MSG_PEEK);
std::cout << "----Bytes: " << bytes << std::endl;
	buffer[bytes] = '\0';
	if (this->checkBytesReceived(bytes) == -1)
		return (-1);
	
	std::string 		str(buffer);
	pos = str.find(this->_delimeter);
	if (pos == std::string::npos) {
		printYellow("I didn't find the delimeter in getHeader");
		return (-1);
	}
	this->_header.append(buffer, pos);
	getContentLength();
	this->_httpMessage = this->_header;
// printYellow("header: " + this->_header);
	return (0);
}

bool		Request::getContentLength() {
	size_t		pos;
	size_t		end;

	pos = this->_header.find("Content-Length: ");
	if (pos == std::string::npos) {
		return true;
	}
	pos += 16;
	end = pos;
//	while ((end != std::string::npos) && (std::isspace(this->_header[end])))
	while ((end < this->_header.size()) && (std::isspace(this->_header[end])))
		end++;
//	while ((end != std::string::npos) && (!std::isspace(this->_header[end])))
	while ((end < this->_header.size()) && (!std::isspace(this->_header[end])))
		end++;
	if (end != this->_header.size())
		this->_contentLength = Utils::atoi(this->_header.substr(pos, (end - pos)));
	// if (this->_contentLength > this->_maxContentLenght) {
		// std::cout << "The size of file is to big" << std::endl;
	// 	return false;
	// }
	return true;
	}

bool        Request::appendFirstBody(std::string const& buffer)
{
    if (this->_body.empty())
    {
		size_t i = buffer.find(this->_delimeter) + this->_delimeter.size();
        this->_body.append(buffer.begin() + i, buffer.end());
        return (true);
    }
    return (false);
}

int		Request::getBody(int client) {
	char		buffer[BUFFER_SIZE];
	int			bytes;
int         cont = 1;

	if (this->_contentLength == 0) {
		this->_ready = true;
		printYellow("The request is ready without body");
		return (0);
	}
    bytes = recv(client, buffer, BUFFER_SIZE - 1, 0);
	buffer[bytes] = '\0';
std::cout << "Round number: " << cont++ << " | Bodysize: " << this->_body.size() << " | I read now: " << bytes << std::endl;
    if (this->checkBytesReceived(bytes) != 1) 
        return (-1);
    if (appendFirstBody(buffer))
        return 0;
    this->_body.append(buffer, bytes);
    if (this->_body.size() == this->_contentLength)
    {
        this->_ready = true;
		this->_httpMessage = this->_header + this->_body;
        printYellow("Reached the size");
        // printYellow("BODY: " + this->_body);
    }
	return (0);
}


bool		Request::receiveFromClient(int client)
{
	if (getHeader(client) < 0)
		return (false);
	if (getBody(client) < 0)
		return (false);
	return (true);
}

/*******************************************************/
/*				Getters of HTTP request.               */
/*******************************************************/

const std::string &		Request::getMethod(void) const
{
	return (this->_method);
}

const std::string &		Request::getLocation(void) const
{
	return (this->_location);
}

const std::string &		Request::getRequestedInf(void) const
{
	return (this->_requestedInf);
}

const std::string &		Request::getContentType(void) const
{
	return (this->_contentType);
}

const std::map<std::string, std::string> &		Request::getQueryString(void) const
{
	return (this->_queryString);
}

/*******************************************************/
/*				Parse of HTTP request.					*/
/*******************************************************/

void 	Request::parseRequest()
{
	size_t			pos;
	std::cout << "parseRequest: " << this->_httpMessage << std::endl; 
	pos = this->_httpMessage.find(" HTTP/");
	if (pos == std::string::npos)
		std::cout << "Error in parseRequest" << std::endl;
	else
		this->splitRequest(this->_httpMessage, pos);
	std::cout << "method: " << this->_method << " location: " << this->_location << " request inf: " << this->_requestedInf << std::endl;

}

void	Request::splitRequest(std::string & fullRequest, size_t & pos) {
	std::vector<std::string>			splitHeadRequest;
	std::vector<std::string>::iterator	i;
	size_t								j;

	splitHeadRequest = Utils::split(fullRequest.substr(0, pos), " \t\n");
	i = splitHeadRequest.begin();
	this->_method = *(i++);
	j = (*i).rfind("/");
	this->_location = (*i).substr(0, (j + 1));
	this->_requestedInf = (*i).substr((j + 1), ((*i).size() - (j + 1)));
	j = this->_requestedInf.find("?");
	if (j != std::string::npos)
	{
		this->parseQueryString(this->_requestedInf.substr((j + 1)));
		this->_requestedInf = this->_requestedInf.substr(0, j);
	}
	j = fullRequest.find("Content-Type:");
	if (j != std::string::npos)
		this->_contentType = fullRequest.substr(j, (fullRequest.size() - j));
}

void	Request::parseQueryString(std::string queryString)
{
	std::vector<std::string>	splitQueryString;
	size_t						i;
	size_t						j;
	std::string					key;
	std::string					value;

	splitQueryString = Utils::split(queryString, "&");
	i = 0;
	while (i < splitQueryString.size())
	{
		j = splitQueryString[i].find("=");
		key = splitQueryString[i].substr(0, j);
		value = splitQueryString[i].substr((j + 1));
		this->_queryString[this->urlDecoder(key)] = this->urlDecoder(value);
		i++;
	}
}

std::string	Request::urlDecoder(const std::string & url)
{
	size_t		i;
	int			hexVal;
	std::string	ret;

	i = 0;
	while (i < url.size())
	{
		if (url.compare(i, 1, "+") == 0)
			ret += ' ';
		else if ((url.compare(i, 1, "%") == 0) && ((i + 2) < url.size()))
		{
			std::istringstream	hexStream(url.substr((i + 1), 2));
			hexStream >> std::hex >> hexVal;
			ret += static_cast<char>(hexVal);
			i += 2;			
		}
		else
			ret += url[i];
		i++;
	}
	return (ret);
}

/*******************************************************/
/*				Reset methods.							*/
/*******************************************************/

void        Request::clearAll() {
	try {
		this->_header.clear();
		this->_body.clear();
		this->_httpMessage.clear();
		// printYellow("Cleaned all data in request");
	}
	catch (std::exception & e) {
		std::cout << "Error in clearAll: " << e.what() << std::endl;
	}
}

void        Request::reset() {
	try {
		this->_ready = false;
		this->_contentLength = 0;
		this->clearAll();
	}
	catch (std::exception & e) {
		std::cout << "Error in reset: " << e.what() << std::endl;
	}
}

bool    	Request::isReady() {
    return (this->_ready);
}
