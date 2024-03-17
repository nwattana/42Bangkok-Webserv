#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>
#include "Webserv.hpp"

class PollServer {
	private:
		std::string port;

		void _set_up();
		void _loop();

		struct sockaddr_storage remoteaddr; // client address
		socklen_t addrlen;
		char buffer[30000]; // buffer for client data
		char remoteIP[INET6_ADDRSTRLEN];
		int listener;
		int newfd;
		struct pollfd *pfds;
		int fd_count = 0;
		int fd_size = 5;

	public:
		PollServer();
		~PollServer();
		void start();
		void stop();
		int get_listener_socket();
		void add_to_pfds(int newfd);
		void *get_in_addr(struct sockaddr *sa);

};