#include "PollServer.hpp"

PollServer::PollServer()
{
	_set_up();
	_loop();
}

PollServer::~PollServer()
{
}

void PollServer::start()
{
}

void PollServer::stop()
{
}

int PollServer::get_listener_socket()
{
	int ye = 1;
	int rv;

	struct addrinfo hints, *ai, *p;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	rv = getaddrinfo(NULL, port.c_str(), &hints, &ai);
	if (rv != 0)
	{
		exit(1);
	}
	p = ai;
	while (p != NULL)
	{
		listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if (listener < 0)
		{
			continue;
		}
		setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &ye, sizeof(int));
		if (bind(listener, p->ai_addr, p->ai_addrlen) < 0)
		{
			close(listener);
			continue;
		}

		break;
		p = p->ai_next;
	}
	freeaddrinfo(ai);
	if (p == NULL)
	{
		exitWithError("Error: failed to bind", 1);
	}
	
	return (listener);
}

void PollServer::_set_up()
{
	fd_count = 0;
	fd_size = 5;
	pfds = new pollfd[fd_size];
	port = "9000";
	listener = get_listener_socket();

	// Set up the initial listening socket
	pfds[0].fd = listener;
	std::cout << "pfds[0].fd: " << pfds[0].fd << std::endl;
	pfds[0].events = POLLIN;
	fd_count = 1;
}

void PollServer::add_to_pfds(int newfd)
{
	if (fd_count == fd_size)
	{
		fd_size *= 2;
		pfds = (struct pollfd *)realloc(pfds, sizeof(*pfds) * (fd_size));
	}
	pfds[fd_count].fd = newfd;
	pfds[fd_count].events = POLLIN;
	(fd_count)++;
}

void *PollServer::get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET)
	{
		return &(((struct sockaddr_in *)sa)->sin_addr);
	}
	return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

void print_pfds(
	struct pollfd *pfds,
	int fd_count)
{
	std::cout << "print_pfds: fd_count: " << fd_count << std::endl;
	for (int i = 0; i < fd_count; i++)
	{
		std::cout << "print_pfds: pfds[" << i << "].fd: " << pfds[i].fd << std::endl;
	}
}

void PollServer::_loop()
{
	int poll_count;
	int i;
	std::string ipstr;

	while (1)
	{
		print_pfds(pfds, fd_count);
		poll_count = poll(pfds, fd_count, -1);
		if (poll_count == -1)
		{
			exitWithError("Error: poll", 1);
		}

		i = 0;
		while (i < fd_count)
		{
			// check if someone's ready
			if (pfds[i].revents & POLLIN)
			{
				if (pfds[i].fd == listener)
				{
					std::cout << "poll Server: new connection" << std::endl;
					addrlen = sizeof(remoteaddr);
					newfd = accept(listener, (struct sockaddr *)&remoteaddr, &addrlen);
					if (newfd == -1)
					{
						exitWithError("Error: accept", 1);
					}
					add_to_pfds(newfd);
					// inet_ntop from arpa/inet.h
					ipstr = inet_ntop(remoteaddr.ss_family, get_in_addr((struct sockaddr *)&remoteaddr), remoteIP, INET6_ADDRSTRLEN);
					std::cout << "poll Server: new connection from " << ipstr << std::endl;
				}
			}
			else
			{
				// if not the listener, must be client
				int nbytes;
				int sender_fd = pfds[i].fd;

				nbytes = recv(sender_fd, buffer, sizeof buffer, 0);
				if (nbytes <= 0)
				{
					// got error or connection closed by client
					if (nbytes == 0)
					{
						// connection closed
						std::cout << "poll Server: socket " << sender_fd << " hung up" << std::endl;
					}
					else
					{
						perror("recv");
					}
					close(sender_fd); // bye!

					// delete from pfds
					pfds[i] = pfds[fd_count - 1];
					fd_count--;
				}
				else
				{
					// we got some data from a client
					for (int j = 0; j < fd_count; j++)
					{
						int dest_fd = pfds[j].fd;
						if (dest_fd == sender_fd)
						{
							write(1, buffer, nbytes);
						}
					}
					std::cout << "poll Server: received " << nbytes << " bytes: " << buffer << std::endl;
				}


			}
		}
		i++;
	}
}

