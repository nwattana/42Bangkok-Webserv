/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkaiyawo <kkaiyawo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 14:35:33 by kkaiyawo          #+#    #+#             */
/*   Updated: 2023/10/22 15:21:06 by kkaiyawo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <iostream>
# include <fstream>
# include <string>
# include <vector>
# include <map>

using std::string;
using std::vector;
using std::map;
using std::fstream;
using std::cin;
using std::cout;
using std::endl;

# include "ConfigParser.hpp"

# ifndef EXIT_FAILURE
#  define EXIT_FAILURE 1
# endif

void	exitWithError(string errorMessage, int mode);

#endif
