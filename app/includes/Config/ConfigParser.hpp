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
# include "Server.hpp"

class ConfigParser {
	public:
		ConfigParser(std::string filename);
		~ConfigParser();
	private:
		ConfigParser();
		ConfigParser(ConfigParser const &src);
		ConfigParser &operator=(ConfigParser const &rhs);

		void read_file_config(void);
		void get_server_config(void);
		void check_config(void);
		int is_allow_directive(std::string str);
		void get_server();

		std::string m_fileName;
		std::string m_raw_config;
		std::fstream m_file;
		std::stringstream ss;
		std::string top_lv_index;
		int bracket;
};

bool IsConfFile(const std::string& filename);

#endif
