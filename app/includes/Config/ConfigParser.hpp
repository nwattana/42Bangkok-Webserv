/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkaiyawo <kkaiyawo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 14:03:13 by kkaiyawo          #+#    #+#             */
/*   Updated: 2024/03/11 14:38:31 by kkaiyawo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include "Webserv.hpp"
#include "ServerConfig.hpp"
#include "LocationBlock.hpp"

#define OPEN_BRACKET 1
#define CLOSE_BRACKET 0

class ConfigParser
{
public:
	ConfigParser(std::string filename);
	~ConfigParser();
	ConfigParser(ConfigParser const &src);
	ConfigParser &operator=(ConfigParser const &rhs);

	void printServerConfig(void) const;
	std::vector<ServerConfig> getServerConfig(void) const;
private:
	ConfigParser();

	int _isSetServer;
	int _isSetLocation;
	int server_bracket;
	int location_bracket;
	std::map<std::string, std::vector<std::string> > root_config;
	std::vector<std::string> allow_directive;
	std::vector<ServerConfig> server_config;
	std::string m_fileName;
	std::string m_raw_config;
	std::fstream m_file;
	std::stringstream ss;

	int is_allow_directive(std::string str);
	void processLocationBlock(std::vector<std::string>::iterator &it, LocationBlock &tmp_location, ServerConfig &tmp_server);
	void processServerBlock(std::vector<std::string>::iterator &it, ServerConfig &tmp_server);
	void handleBrackets(const std::string& token, ServerConfig& tmp_server);
	void read_file_config(void);
};

bool IsConfFile(const std::string &filename);

#endif
