#include "Request.hpp"

Request::Request(void) {}

Request	&Request::operator=(const Request & src)
{
	if (this != &src)
	{
		this->_method = src._method;
		this->_location = src._location;
		this->_requestedInf = src._requestedInf;
		this->_contentType = src._contentType;
		this->_queryString = src._queryString;
	}
	return (*this);
}

Request::Request(const Request & copy)
{
	*this = copy;
	return ;
}

Request::~Request(void) {}

int	checkBytesReceived(ssize_t bytes_received)
{
	if (bytes_received == -1)
	{
		std::cout << "Error in recv: " << strerror(errno) << std::endl;
		return (-1);
	}
	else if (bytes_received == 0)
	{
		std::cout << "Client disconnected" << std::endl;
		return (0);
	}
	return (1);
}


static void printYellow(std::string const& str) {
	std::cout << "\033[1;33m" << str << "\033[0m" << std::endl;
}

int		Request::receiveFromClient(int client)
{
	char		buffer[BUFFER_SIZE];
	size_t		contentLenght;
	std::string initBody = "\r\n\r\n";
	int			bytes;

	// Get header.
	while ((bytes = recv(client, buffer, BUFFER_SIZE - 1, MSG_PEEK | MSG_DONTWAIT)) > 0) 
	{
		//printYellow("bytes: ", bytes);
		if (checkBytesReceived(bytes) != 1) return (-1);
		char*	msg_pos = std::search(buffer, buffer + bytes, initBody.begin(), initBody.end());
		if (msg_pos != buffer + bytes) 
		{
			// Found crlf in buff.
			std::string str(buffer);
			size_t i = str.find(initBody);
			this->_header.append(buffer, i);
			break;
		}
		else 
		{
			// crlf not found in buff.
			this->_header.append(buffer, bytes);
			recv(client, buffer, BUFFER_SIZE - 1, MSG_DONTWAIT);
		}
	}
	// printYellow("header: " + this->_header);

	//get content lenght.
	contentLenght = 149438;
	// std::string	phraseLenght("Content-Length");
	// char*	lineWithLen = search(this->_header.begin(),
	// 							this->_header.end(),
	// 							phraseLenght.begin(),
	// 							phraseLenght.end());
	// lineWithLen += 16;
	// while (is_number(lineWithLen*)) {
	// 	lineWithLen++;
	// 	contentLenght += atoi(lineWithLen*) * 10;
	// }

	while (this->_body.size() < contentLenght)
	{
		bytes = recv(client, buffer, BUFFER_SIZE - 1, MSG_DONTWAIT);
		if (checkBytesReceived(bytes) != 1) return (-1);
		if (this->_body.empty())
		{
			// The first time needs to jump to begin of body.
			// char*	msg_pos = std::search(buffer, buffer + bytes, initBody.begin(), initBody.end());
			std::string str(buffer);
			size_t i = str.find(initBody);
			this->_body.append(buffer, i + initBody.size());
		}
		this->_body.append(buffer, bytes);
	}
	printYellow("BODY: " + this->_body);

	this->_httpMessage = this->_header + this->_body;
	// std::cout << "###### REQUEST ######" << std::endl << this->_httpMessage << std::endl;
	return (1);
}

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

void 	Request::parseRequest()
{
//	std::cout << "start | parserRequest: " << request << std::endl;
	size_t		pos;
	size_t		end;

	pos =this-> _httpMessage.find("\r\n\r\n");
	if (pos != std::string::npos)
	{
		this->_headRequest =this-> _httpMessage.substr(0, pos);
		pos += 4;
		if (pos <this-> _httpMessage.size())
			this->_bodyRequest =this-> _httpMessage.substr(pos);
	}
	else
		this->_headRequest =this-> _httpMessage.substr(0);
	pos = this->_headRequest.find("Content-Length: ");
	pos += 16;
	end = pos;
	while ((end != std::string::npos) && (std::isspace(this->_headRequest[end])))
		end++;
	while ((end != std::string::npos) && (!std::isspace(this->_headRequest[end])))
		end++;
	if (end != this->_headRequest.size())
		this->_contentLength = Utils::atoi(this->_headRequest.substr(pos, (end - pos)));
	pos = this->_headRequest.find(" HTTP/");
	if (pos == std::string::npos)
		std::cout << "Erro na requisição" << std::endl;
	else
		this->splitRequest(this->_headRequest, pos);
//	std::cout << "end   | parserRequest" << std::endl;
}

/*
void 	Request::parseRequest()
{
//	std::cout << "start | parserRequest: " << request << std::endl;
	size_t			pos;

	pos = this->_httpMessage.find(" HTTP/");
	if (pos == std::string::npos)
		std::cout << "Erro na requisição" << std::endl;
	else
		this->splitRequest(this->_httpMessage, pos);
//	std::cout << "end   | parserRequest" << std::endl;
}
*/

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