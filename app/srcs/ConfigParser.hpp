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

class ConfigParser {
	public:
		ConfigParser(std::string filename);
		~ConfigParser();
	private:
		ConfigParser();
		ConfigParser(ConfigParser const &src);
		ConfigParser &operator=(ConfigParser const &rhs);

		std::string m_fileName;
		std::string m_config;
		std::fstream m_file;
};

bool IsConfFile(const std::string& filename);

#endif
