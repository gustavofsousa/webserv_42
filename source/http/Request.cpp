/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserRequest.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: coder <coder@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 15:23:54 by woliveir          #+#    #+#             */
/*   Updated: 2022/05/09 15:09:49 by coder            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

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
	}
	return (*this);
}

Request::Request(const Request & copy)
{
	*this = copy;
	return ;
}

Request::~Request(void) {}

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
	// std::cout << "start | splitRequest | fullRequest: " << std::endl << fullRequest << " pos: " << pos << std::endl;
	std::vector<std::string>			splitHeadRequest;
	std::vector<std::string>::iterator	i;
	size_t								j;

	splitHeadRequest = Utils::split(fullRequest.substr(0, pos), " \t\n");
	i = splitHeadRequest.begin();
	this->_method = *(i++);
	j = (*i).rfind("/");
	this->_location = (*i).substr(0, (j + 1));
	this->_requestedInf = (*i).substr((j + 1), ((*i).size() - (j + 1)));
	j = fullRequest.find("Content-Type:");
	if (j == std::string::npos)
		std::cout << "there is not content type" << std::endl;
	else
		this->_contentType = fullRequest.substr(j, (fullRequest.size() - j));
//	std::cout << "end   | splitRequest Content-Type: " << j << " contentType: " << this->_contentType << std::endl;
}
