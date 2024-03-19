#pragma once
#include "Webserv.hpp"
#include "SelectServer.hpp"
#include <map>

class ServerCluster
{
	private:
		fd_set _master;
		fd_set _read_fds;
		std::vector<SelectServer> _servers;
		std::vector<int> _sfds; // server file descriptors
		std::map<int, int> _cluster_router; // client_fd -> server_fd
		int fd_max;

	public:
		ServerCluster();
		~ServerCluster();

		void addServer(SelectServer server);
		void addServerClient(int server_fd, int client_fd);
		int is_server(int socket);
		SelectServer * get_server(int fd);
		void run(void);
};