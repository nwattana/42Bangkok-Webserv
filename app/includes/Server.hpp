#pragma once

#include <string>
#include "Webserv.hpp"
#include <exception>
#include <vector>

#define MAX_CLIENTS 14

class Server
{
	private:
		std::string			raw_config;
		std::vector<int>	listen;

		Server(void);
		~Server(void);

	public:
		Server(std::string config);
		Server &operator=(Server const &src);

		std::string get_config(void) const;
};
