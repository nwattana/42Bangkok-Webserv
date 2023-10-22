/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkaiyawo <kkaiyawo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 14:24:16 by kkaiyawo          #+#    #+#             */
/*   Updated: 2023/10/22 14:53:47 by kkaiyawo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"

ConfigParser::ConfigParser(string filename) : m_fileName(filename)
{
	m_file.open(m_fileName);
	if (!m_file.is_open())
	{
		//error msg goes here
		exitWithError("Error opening file", 0);
	}
	string line;
	while (std::getline(m_file, line))
	{
		m_config += line;
		m_config += "\n";
	}
	m_file.close();
	cout << m_config;
}

ConfigParser::~ConfigParser()
{
}

ConfigParser::ConfigParser() : m_fileName("srcs/webserv.conf")
{
	m_file.open(m_fileName);
	if (!m_file.is_open())
	{
		//error msg goes here
		exitWithError("Error opening file", 0);
	}
	string line;
	while (std::getline(m_file, line))
	{
		m_config += line;
		m_config += "\n";
	}
	m_file.close();
	cout << m_config;
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
