#include "Request.hpp"

/*******************************************************/
/*						Constructors.		           */
/*******************************************************/


Request::Request(int newClient, ConfigFile _configFile){
	this->_parser = new RequestParser(*this);
	this->_serverConf = _configFile;
    this->_fromClient = newClient;
	this->_info.contentLength = -1;
	this->_delimeter = "\r\n\r\n";
	this->_ready = false;
	this->reset();
}

Request&			Request::operator=(Request const & other) {
	if (this != &other) {
		this->_info = other._info;
		this->_serverConf = other._serverConf;
		this->_parser = other._parser;
		this->_delimeter = other._delimeter;
		this->_ready = other._ready;
		this->_header = other._header;
		this->_body = other._body;
	}
	return (*this);
}

Request::Request(Request const & other) {
	this->_parser = new RequestParser(*this);
	*this = other;
}

Request::~Request(void) {
	delete this->_parser;
}

static void printYellow(std::string const& str) {
	std::cout << "\033[1;33m" << str << "\033[0m" << std::endl;
}

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

int		Request::extractHeader(std::string const& buffer) {
	size_t		pos;

    if (this->_header.empty() == false) {
        return (0);
	}
	pos = buffer.find(this->_delimeter);
	if (pos == std::string::npos) {
		printYellow("I didn't find the delimeter in getHeader");
		return (-1);
	}
	this->_header.append(buffer.begin(), buffer.begin() + pos);
// printYellow("header: " + this->_header);
	return (0);
}

bool        Request::appendTheBody(std::string const& buffer, int bytes)
{
	size_t i;

    if (this->_body.empty())
    {
		i = buffer.find(this->_delimeter) + this->_delimeter.size();
        this->_body.append(buffer.begin() + i, buffer.end());
    }
	else
	{
		this->_body.append(buffer.begin(), buffer.begin() + bytes);
	}
	return (true);
}

int		Request::extractBody(std::string const& buffer, int bytes)
{
    this->appendTheBody(buffer, bytes);
    if (this->_body.size() == this->_info.contentLength)
    {
        this->_ready = true;
		this->_header = this->_header + this->_body;
printYellow("Reached the size");
// printYellow("BODY: " + this->_body);
    }
	return (0);
}


bool		Request::receiveFromClient(int client)
{
	char		buffer[BUFFER_SIZE];
	int			bytes;

	bytes = recv(client, buffer, BUFFER_SIZE - 1, 0);
    if (this->checkBytesReceived(bytes) != 1) return (false);
	buffer[bytes] = '\0';
std::cout << "Round:" << " Bodysize: " << this->_body.size() << " | I read now: " << bytes << std::endl;
	if (this->extractHeader(buffer) < 0 || this->_parser->parse() == false)
		return (false);
	if (this->extractBody(buffer, bytes) < 0)
		return (false);
	return (true);
}

/*******************************************************/
/*				Reset methods.							*/
/*******************************************************/

void        Request::clearAll() {
	try {
		this->_header.clear();
		this->_body.clear();
		this->_header.clear();
		// printYellow("Cleaned all data in request");
	}
	catch (std::exception & e) {
		std::cout << "Error in clearAll: " << e.what() << std::endl;
	}
}

void        Request::reset() {
	try {
		this->_ready = false;
		this->_info.contentLength = 0;
		this->clearAll();
	}
	catch (std::exception & e) {
		std::cout << "Error in reset: " << e.what() << std::endl;
	}
}

bool    	Request::isReady() {
    return (this->_ready);
}


/*******************************************************/
/*				Getters of HTTP request.               */
/*******************************************************/

int         Request::getContentLength() {
    return (this->_info.contentLength);
}

const std::string &		Request::getMethod(void) const
{
	return (this->_info.method);
}

const std::string &		Request::getLocation(void) const
{
	return (this->_info.location);
}

const std::string &		Request::getRequestedInf(void) const
{
	return (this->_info.requestedInf);
}

const std::string &		Request::getContentType(void) const
{
	return (this->_info.contentType);
}

const std::map<std::string, std::string> &		Request::getQueryString(void) const
{
	return (this->_info.queryString);
}
const ConfigFile  &		Request::getServerConf(void) const
{
	return (this->_serverConf);
}

const std::string &		Request::getUserAgent(void) const
{
	return (this->_info.userAgent);
}

const std::string &		Request::getHost(void) const
{
	return (this->_info.host);
}

const std::string &		Request::getQueryStringS(void) const
{
	return (this->_info.queryStringS);
}

const std::string &		Request::getHeader(void) const
{
	return (this->_header);
}

const std::string &		Request::getBody(void) const
{
	return (this->_body);
}

/*******************************************************/
/*				Setters of HTTP request.               */
/*******************************************************/

void					Request::setMethod(std::string newMethod)
{
	this->_info.method = newMethod;
}

void					Request::setLocation(std::string newLocation)
{
	this->_info.location = newLocation;
}

void					Request::setRequestedInf(std::string newInfo)
{
	this->_info.requestedInf = newInfo;
}

void					Request::setUserAgent(std::string newAgent)
{
	this->_info.userAgent = newAgent;
}

void					Request::setHost(std::string newHost)
{
	this->_info.host = newHost;
}

void					Request::setContentType(std::string newContentType)
{
	this->_info.contentType = newContentType;
}

void					Request::setQueryStringS(std::string newQueryStringS)
{
	this->_info.queryStringS = newQueryStringS;
}

void					Request::setQueryString(std::string key, std::string value)
{
	this->_info.queryString[key] = value;
}

void					Request::setContentLength(int value) {
	this->_info.contentLength = value;
}




