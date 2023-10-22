/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkaiyawo <kkaiyawo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 14:03:13 by kkaiyawo          #+#    #+#             */
/*   Updated: 2023/10/22 14:52:53 by kkaiyawo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGPARSER_HPP
# define CONFIGPARSER_HPP

# include "Webserv.hpp"

class ConfigParser {
	public:
		ConfigParser(string filename);
		~ConfigParser();
	private:
		ConfigParser();
		ConfigParser(ConfigParser const &src);
		ConfigParser &operator=(ConfigParser const &rhs);

		string m_fileName;
		string m_config;
		fstream m_file;
};

#endif
