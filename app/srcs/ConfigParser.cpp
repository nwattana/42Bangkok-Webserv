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

bool IsConfFile(const std::string &filename)
{
	if (filename.length() < 5)
		return false;
	std::string extension = filename.substr(filename.length() - 5);
	return extension == ".conf";
}

ConfigParser::ConfigParser(std::string filename) : m_fileName(filename)
{
	this->bracket = 0;
	if (!IsConfFile(m_fileName))
	{
		// error msg goes here
		exitWithError("Wrong extension (must be .conf)", 0);
	}
	try
	{
		this->read_file_config();
		this->check_config();
		// this->get_server_config();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	std::cout << "root index = "<< this->top_lv_index << std::endl;
}

ConfigParser::~ConfigParser()
{
}

/// @brief Default constructor private member fucntion cant be called
ConfigParser::ConfigParser() : m_fileName("srcs/webserv.conf")
{
	std::string filename = "nginx.conf";
	if (!IsConfFile(filename))
	{
		// error msg goes here
		exitWithError("Wrong extension (must be .conf)", 0);
	}
	this->read_file_config();
}

ConfigParser::ConfigParser(ConfigParser const &src)
{
	*this = src;
}

ConfigParser &ConfigParser::operator=(ConfigParser const &rhs)
{
	if (this != &rhs)
	{
		// copy
	}
	return (*this);
}

/// @brief Read Config file and keep this m_raw_config
/// @param
void ConfigParser::read_file_config(void)
{
	m_file.open(m_fileName.c_str());
	if (!m_file.is_open())
	{
		// error msg goes here
		exitWithError("Error opening file", 0);
	}
	std::string line;
	while (std::getline(m_file, line))
	{
		// Comment
		if (line[0] == '#')
			continue;
		this->m_raw_config += line;
		this->m_raw_config += '\n';
	}
	m_file.close();
}

void ConfigParser::get_server_config(void)
{
	std::stringstream ss;
	std::string str;
	int i = 20;

	ss << m_raw_config;
	while (ss >> str)
	{
		std::cout << str << std::endl;
	}
}


// check top level directive
int ConfigParser::is_allow_directive(std::string str)
{
	if (str.compare("server") == 0)
	{
		printf("server");
		return (SERVER);
	}
	if (str.compare("server{") == 0)
	{
		printf("server");
		this->bracket += 1;
		return (SERVER);
	}
	if (str.compare("index") == 0)
	{
		return (INDEX);
	}
	return (0);
}

void ConfigParser::check_config(void)
{
	std::string directive;
	int d_type;

	ss << this->m_raw_config;
	while (ss >> directive)
	{
		d_type = is_allow_directive(directive);
		if (d_type == SERVER)
		{
			std::cout << "\nGET SERVER\n";
			this->get_server();
			std::cout << "\n\n end server \n\n";
		}
		if (d_type == INDEX)
		{
			ss >> directive;
			this->top_lv_index = directive;
		}
	}
	while (ss >> directive)
	{
		std::cout << directive << std::endl;
	}
}

void ConfigParser::get_server()
{
	std::string token;

	while(ss >> token)
	{
		std::cout << token << std::endl;
		if (token.compare("{") == 0)
			this->bracket += 1;
		else if (token.compare("}") == 0)
			this->bracket -= 1;
		else if (token.compare("location") == 0)
		{
			printf("add ti location\n");
			// this->location_update();

		}

		if (this->bracket == 0)
			return ;
		if (this->bracket < 0)
			throw std::runtime_error("invalid bracket");
	
	}
}
