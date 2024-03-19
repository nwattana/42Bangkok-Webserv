#include "SelectServer.hpp"

SelectServer::SelectServer()
{
	_port = "8080";
}

SelectServer::SelectServer(std::string port) : _port(port)
{
}

SelectServer::~SelectServer()
{
	std::cout << "Closing server socket" << std::endl;
}

int SelectServer::getServerSocket(void)
{
	return _serverSocket;
}

int SelectServer::setupServer(void)
{
	_setup();
	return (0);
}

void SelectServer::_setup()
{
	s_get_addrinfo();
	s_create_socket();
	s_bind_socket();
	s_listen_socket();
}

void SelectServer::s_get_addrinfo(void)
{
	memset(&_hints, 0, sizeof(_hints));
	_hints.ai_family = AF_INET;
	_hints.ai_socktype = SOCK_STREAM;
	_hints.ai_flags = AI_PASSIVE;

	int status = getaddrinfo(NULL, _port.c_str(), &_hints, &_res);
	if (status != 0)
	{
		fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
	}
	std::cout << "Get address info: " << status << std::endl;
}

void SelectServer::s_create_socket(void)
{
	_serverSocket = socket(_res->ai_family, _res->ai_socktype, _res->ai_protocol);
	if (_serverSocket < 0)
	{
		perror("socket");
	}
	std::cout << "Create socket: " << _serverSocket << std::endl;
}

void SelectServer::s_bind_socket(void)
{
	socklen_t addr_size;
	server_addr.sin_family = AF_UNSPEC;
	server_addr.sin_port = htons(atoi(_port.c_str()));
	server_addr.sin_addr.s_addr = INADDR_ANY;
	addr_size = sizeof(server_addr);
	if (bind(_serverSocket, _res->ai_addr, _res->ai_addrlen) < 0)
	{
		perror("bind");
	}
	std::cout << "Bind to port " << _port << std::endl;
}

void SelectServer::s_listen_socket(void)
{
	if (listen(_serverSocket, _backlog_length) < 0)
	{
		perror("listen");
	}
	std::cout << "Listening on port " << _port << std::endl;
}

int SelectServer::acceptSocket(void)
{
	int new_fd;
	socklen_t _client_addr_len;
	struct sockaddr_storage _client_addr;

	
	new_fd = accept(_serverSocket, (struct sockaddr *)&_client_addr, &_client_addr_len);
	if (new_fd < 0)
	{
		perror("accept");
		return (-1);
	}
	else
	{
		_clientSockets.push_back(new_fd);
	}
	return new_fd;
}

// when got message from client use this function
int SelectServer::requestHandler(int clientSocket)
{
	std::cout << "recived request from client_fd: " << clientSocket << std::endl;
	return clientSocket;
}

int SelectServer::is_new_connection(int socket)
{
	int i;
	for (i = 0; i <= _clientSockets.size(); i++)
	{
		if (_clientSockets[i] == socket)
			return 0;
	}
	return 1;
}