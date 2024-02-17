/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkaiyawo <kkaiyawo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 14:24:16 by kkaiyawo          #+#    #+#             */
/*   Updated: 2023/10/22 15:26:15 by kkaiyawo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"

bool IsConfFile(const std::string& filename) {
	if (filename.length() < 5)
		return false;
	std::string extension = filename.substr(filename.length() - 5);
	return extension == ".conf";
}

ConfigParser::ConfigParser(std::string filename) : m_fileName(filename)
{
	if (!IsConfFile(m_fileName)) {
		//error msg goes here
		exitWithError("Wrong extension (must be .conf)", 0);
	}
	m_file.open(m_fileName.c_str());
	if (!m_file.is_open())
	{
		//error msg goes here
		exitWithError("Error opening file", 0);
	}
	std::string line;
	while (std::getline(m_file, line))
	{
		m_config += line;
		m_config += "\n";
	}
	m_file.close();
	std::cout << m_config;
}

ConfigParser::~ConfigParser()
{
}






ConfigParser::ConfigParser() : m_fileName("srcs/webserv.conf")
{
	std::string filename = "nginx.conf";
	if (!IsConfFile(filename)) {
		//error msg goes here
		exitWithError("Wrong extension (must be .conf)", 0);
	}
	m_file.open(m_fileName.c_str());
	if (!m_file.is_open()) {
		//error msg goes here
		exitWithError("Error opening file", 0);
	}
	std::string line;
	while (std::getline(m_file, line))
	{
		m_config += line;
		m_config += "\n";
	}
	m_file.close();
	std::cout << m_config;
}

ConfigParser::ConfigParser(ConfigParser const &src)
{
	*this = src;
}

ConfigParser &ConfigParser::operator=(ConfigParser const &rhs)
{
	if (this != &rhs)
	{
		//copy
	}
	return (*this);
}
