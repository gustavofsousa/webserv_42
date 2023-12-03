/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: coder <coder@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 15:23:54 by woliveir          #+#    #+#             */
/*   Updated: 2022/05/09 15:09:49 by coder            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location(void)
{
	this->_methods.push_back("GET");
	this->_methods.push_back("DELETE");
	this->_methods.push_back("POST");
}

Location	&Location::operator=(const Location & src)
{
	if (this != &src)
	{
		this->_path = src._path;
		this->_allowed_methods = src._allowed_methods;
		this->_index = src._index;
	}
	return (*this);
}

Location::Location(const Location & copy)
{
	*this = copy;
	return ;
}

Location::~Location(void) {}

const std::string &					Location::getPath(void) const
{
	return (this->_path);
}

const std::vector<std::string>	&	Location::getMethods(void) const
{
	return (this->_allowed_methods);
}

const std::vector<std::string>	&	Location::getIndex(void) const
{
	return (this->_index);
}

void								Location::setPath(std::string & path)
{
//	std::cout << "int     de setPath: " << path << std::endl;
	this->_path = path;
//	std::cout << "end     de setPath: " << this->_path << std::endl;
}

void								Location::setMethods(std::vector<std::string> & vecLocation)
{
//	std::cout << "int     de setMethods: " << vecLocation[0] << std::endl;
	this->_allowed_methods = vecLocation;
/*
	size_t j = 0;
//	std::cout << "setMethods: ";
	while(j < this->_allowed_methods.size())
	{
		std::cout << this->_allowed_methods[j] << " ";
		j++;
	}
	std::cout << std::endl;
*/
//	std::cout << "end     de setMethods: " << vecLocation[0] << std::endl;
}

void								Location::setIndex(std::vector<std::string> & vecIndex)
{
//	std::cout << "int     de setIndex: " << vecIndex[0] << std::endl;
	this->_index = vecIndex;
/*
	size_t j = 0;
//	std::cout << "setIndex: ";
	while(j < this->_index.size())
	{
		std::cout << this->_index[j] << " ";
		j++;
	}
	std::cout << std::endl;
*/
//	std::cout << "end     de setIndex: " << vecIndex[0] << std::endl;
}