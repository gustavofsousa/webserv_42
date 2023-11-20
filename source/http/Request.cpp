/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gusta <gusta@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 15:23:54 by woliveir          #+#    #+#             */
/*   Updated: 2023/11/20 09:31:30 by gusta            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request(void) {}

Request::Request(std::string buffer)
{
	this->parserRequest(buffer);
}

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


int		Request::receiveFromClient(int client) {

	char		buffer[4096];
	ssize_t 	bytes_received;
	bytes_received = recv(client, buffer, sizeof(buffer), 0);

	buffer[bytes_received] = '\0';
	std::cout << "###### REQUEST ######" << std::endl << buffer << std::endl;
	return (bytes_received);
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

void 	Request::parserRequest(std::string & request)
{
//	std::cout << "start | parserRequest: " << request << std::endl;
	size_t			pos;

	pos = request.find(" HTTP/");
	if (pos == std::string::npos)
		std::cout << "Erro na requisição" << std::endl;
	else
		this->splitRequest(request, pos);
//	std::cout << "end   | parserRequest" << std::endl;
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