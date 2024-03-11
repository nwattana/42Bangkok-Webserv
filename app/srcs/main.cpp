/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkaiyawo <kkaiyawo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 13:52:10 by kkaiyawo          #+#    #+#             */
/*   Updated: 2024/03/11 15:39:40 by kkaiyawo         ###   ########.fr       */
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
	std::cout << std::endl;
	//create server
	int server_fd;
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		exitWithError("Error: unable to create a socket", 1);
	}

	sockaddr_in sockaddr;
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(8080);
	sockaddr.sin_addr.s_addr = INADDR_ANY;
	if ((bind(server_fd, (struct sockaddr *)&sockaddr, sizeof(sockaddr))) < 0) {
		exitWithError("Error: unable to bind to port 8080", 1);
	}

	if ((listen(server_fd, 3)) < 0) {
		exitWithError("Error: unable to listen", 1);
	}

	int acceptedSocket;
	if ((acceptedSocket = accept(server_fd, (struct sockaddr *)&sockaddr, (socklen_t *)&sockaddr)) < 0) {
		exitWithError("Error: unable to accept connection", 1);
	}

	char buffer[100];
	if (read(acceptedSocket, buffer, 100) < 0) {
		exitWithError("Error: unable to read from socket", 1);
	}
	std::cout << "Message from client: " << buffer << std::endl;

	std::string response = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
	if (send(acceptedSocket, response.c_str(), response.length(), 0) < 0) {
		exitWithError("Error: unable to send response", 1);
	}

	close(acceptedSocket);
	close(server_fd);
	std::cout << "\nEnd program all Success\n" << std::endl;
	return (0);
}

