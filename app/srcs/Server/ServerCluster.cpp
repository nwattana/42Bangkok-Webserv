#include "ServerCluster.hpp"

ServerCluster::ServerCluster()
{
	FD_ZERO(&_master);
}

ServerCluster::~ServerCluster()
{
}


void ServerCluster::addServer(SelectServer server)
{
	_servers.push_back(server);
	FD_SET(server.getServerSocket(), &_master);
	_sfds.push_back(server.getServerSocket());
	fd_max = server.getServerSocket();


	std::cout << "Add Server to cluster with server_fd: " << server.getServerSocket() << std::endl;
}

void ServerCluster::addServerClient(int server_fd, int client_fd)
{
	FD_SET(client_fd, &_master);
	_cluster_router[client_fd] = server_fd;
	std::cout << "Add client to server_fd: " << server_fd << " with client_fd: " << client_fd << std::endl;
}

void ServerCluster::run(void)
{
	std::vector<SelectServer>::iterator s_iter;
	int i;
	_read_fds = _master;
	printf("fd_max: %d\n", fd_max);
	if (select(fd_max + 1, &_read_fds, NULL, NULL, NULL) == -1)
	{
		perror("select");
		exit(4);
	}
	for (i = 0; i <= fd_max; i++)
	{
		if (FD_ISSET(i, &_read_fds))
		{
			std::cout << "SelectServer: new connection from " << i << std::endl;
			// check if i = server , if  
			if (is_server(i))
			{
				std::cout << "SelectServer: new connection from server " << i << std::endl;
				int new_fd = (*get_server(i)).acceptSocket();
				if (new_fd < 0)
				{
					perror("accept");
				}
				else
				{
					addServerClient(i, new_fd);
				}
			}
			else
			{
				int nbytes;
				char buf[30000];
				if ((nbytes = recv(i, buf, sizeof(buf), 0)) <= 0)
				{
					if (nbytes == 0)
					{
						std::cout << "selectserver: socket " << i << " hung up" << std::endl;
					}
					else
					{
						perror("recv");
					}
					close(i);
					FD_CLR(i, &_master);
				}
				else
				{
					write(i, buf, nbytes);
				}
			}
		}
	}

	return ;
}

int ServerCluster::is_server(int socket)
{
	int i;
	for (i = 0; i <= _sfds.size(); i++)
	{
		if (_sfds[i] == socket)
			return 1;
	}
	return 0;
}

SelectServer * ServerCluster::get_server(int fd)
{
	int i;
	for (i = 0; i <= _sfds.size(); i++)
	{
		if (_servers[i].getServerSocket() == fd)
			return &_servers[i];
	}
	throw std::runtime_error("Server not found in cluster");
}