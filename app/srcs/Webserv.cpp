/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkaiyawo <kkaiyawo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 14:36:56 by kkaiyawo          #+#    #+#             */
/*   Updated: 2023/10/22 14:45:02 by kkaiyawo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

void	exitWithError(std::string errorMessage, int mode)
{
	// if (mode == 1)
	// 	perror(errorMessage);
	if (mode == 0)
		std::cerr << errorMessage << std::endl;
	exit(EXIT_FAILURE);
}
