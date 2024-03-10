/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkaiyawo <kkaiyawo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 14:03:13 by kkaiyawo          #+#    #+#             */
/*   Updated: 2023/10/22 15:26:40 by kkaiyawo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGPARSER_HPP
# define CONFIGPARSER_HPP

# include "Webserv.hpp"
# include "ServerConfig.hpp"
# include "Location.hpp"

# define OPEN_BRACKET 1
# define CLOSE_BRACKET 0

class ConfigParser {
	public:
		ConfigParser(std::string filename);
		~ConfigParser();

	private:
		ConfigParser();
		ConfigParser(ConfigParser const &src);
		ConfigParser &operator=(ConfigParser const &rhs);

		std::map<std::string, std::vector<std::string>> root_config;
		std::vector<std::string> allow_directive;
		std::vector<ServerConfig> server_config;

		void read_file_config(void);
		int is_allow_directive(std::string str);

		std::string m_fileName;
		std::string m_raw_config;
		std::fstream m_file;
		std::stringstream ss;

		int _isSetServer;
		int _isSetLocation;
		int server_bracket;
		int location_bracket;
};

bool IsConfFile(const std::string& filename);

#endif
