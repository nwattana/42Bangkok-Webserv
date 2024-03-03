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
# include <cstring>
# include <stdlib.h>
# include <cctype>
# include <sstream>
# include <map>

# include "ConfigParser.hpp"

# ifndef EXIT_FAILURE
#  define EXIT_FAILURE 1
# endif

# define SERVER 1
# define INDEX 2


void	exitWithError(std::string errorMessage, int mode);

#endif
