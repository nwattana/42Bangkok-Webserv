#pragma once
#include <iostream>
#include <cstring>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <vector>

class SelectServer
{
private:
	std::string _port;
	int _serverSocket;
	std::vector<int> _clientSockets;

	struct addrinfo _hints;
	struct addrinfo *_res;
	struct sockaddr_in server_addr;
	unsigned int _backlog_length;
	bool _running;

	SelectServer();
	void _setup();
	void s_get_addrinfo(void);
	void s_create_socket(void);
	void s_bind_socket(void);
	void s_listen_socket(void);



public:
	SelectServer(std::string port);
	~SelectServer();

	int setupServer(void);
	int getServerSocket(void);
	int acceptSocket(void);
	int requestHandler(int clientSocket);
	int is_new_connection(int socket);
};

