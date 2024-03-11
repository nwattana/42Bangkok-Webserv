/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkaiyawo <kkaiyawo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 13:52:10 by kkaiyawo          #+#    #+#             */
/*   Updated: 2024/03/11 11:51:03 by kkaiyawo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

int main(int argc, char **argv)
{
	//set filename
	if (argc > 2)
	{
		//error msg goes here
		return (1);
	}
	std::string filename = (argc == 2) ? argv[1] : "webserv.conf";

	ConfigParser config(filename);
	config.printServerConfig();
	//create server
	int server_fd;
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		exitWithError();
	}

	//run server

	close(server_fd);
	std::cout << "\nEnd program all Success\n" << std::endl;
	return (0);
}

