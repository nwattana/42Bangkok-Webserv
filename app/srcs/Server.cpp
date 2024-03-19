#include "Server.hpp"

Server::Server()
{
	this->m_port = "8080";

	this->m_myAddr.sin_family = AF_INET;
	this->m_myAddr.sin_port = htons(atoi(this->m_port.c_str()));
	this->m_myAddr.sin_addr.s_addr = INADDR_ANY;

	this->m_serverInfo = NULL;

	FD_ZERO(&this->m_socketSet);
}

Server::~Server()
{
}

Server::Server(Server const &src)
{
	*this = src;
}

Server &Server::operator=(Server const &rhs)
{
	if (this != &rhs)
	{
		// copy
	}
	return *this;
}

int Server::connectServer(void)
{
	if (this->_setupServer() < 0) {
		this->_closeServer();
		exitWithError("Error: unable to setup server", 1);
	}
	printLog("Server setup successfully");
	this->_printAddressInfo(this->m_serverInfo);
	if (this->_createSocket() < 0) {
		this->_closeServer();
		exitWithError("Error: unable to create a socket", 1);
	}
	printLog("Socket created successfully");
	if (this->_bindAddress() < 0) {
		this->_closeServer();
		// std::string errorMsg = "Error: unable to bind to port " + this->m_port;
		// exitWithError(errorMsg, 1);
		exitWithError();
	}
	printLog("Binded to port " + this->m_port);
	if (this->_listenSocket() < 0) {
		this->_closeServer();
		exitWithError("Error: unable to listen", 1);
	}
	printLog("Listening on port " + this->m_port);

	this->_handleConnection();

	this->_closeServer();
	return 0;
}

int Server::_setupServer(void)
{
	struct addrinfo hints;
	char *port = (char *)this->m_port.c_str();
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC; // use AF_INET6 to force IPv6
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // use my IP
	return getaddrinfo(NULL, port, &hints, &this->m_serverInfo);
}

void Server::_printAddressInfo(struct addrinfo *p)
{
	char ipstr[INET6_ADDRSTRLEN];
	void *addr;
	std::string ipver;

	while (p != NULL)
	{

		// get the pointer to the address itself,
		// different fields in IPv4 and IPv6:
		if (p->ai_family == AF_INET) { // IPv4
			struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
			addr = &(ipv4->sin_addr);
			ipver="IPv4";
		}
		else { // IPv6
			struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
			addr = &(ipv6->sin6_addr);
			ipver="IPv6";
		}

		// convert the IP to a string and print it:
		inet_ntop(p->ai_family, addr, ipstr, sizeof(ipstr));
		std::cout << "  " << ipver << ": " << ipstr << std::endl;

		// show content in struct
		std::cout << "  ai_family: " << p->ai_family << std::endl;
		std::cout << "  ai_socktype: " << p->ai_socktype << std::endl;
		std::cout << "  ai_protocol: " << p->ai_protocol << std::endl;
		std::cout << "  ai_addrlen: " << p->ai_addrlen << std::endl;
		// std::cout << "  ai_canonname: " << p->ai_canonname << std::endl;
		std::cout << "  ai_addr: " << p->ai_addr << std::endl;
		std::cout << "  ai_next: " << p->ai_next << std::endl;
		p = p->ai_next;
	}
}

#include <string> // Include the <string> header

int Server::_createSocket(void)
{
	int yes = 1;
	this->m_sockfd = socket(this->m_serverInfo->ai_family, this->m_serverInfo->ai_socktype, this->m_serverInfo->ai_protocol);
	this->m_fdList.push_back(this->m_sockfd);
	sort(this->m_fdList.begin(), this->m_fdList.end());
	fcntl(this->m_sockfd, F_SETFL, O_NONBLOCK);
	setsockopt(this->m_sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
	printLog("Socket created successfully at fd " + (this->m_sockfd));
	return this->m_sockfd;
}

int Server::_bindAddress(void)
{
	return bind(this->m_sockfd, (struct sockaddr *)&this->m_myAddr, sizeof(this->m_myAddr));
}

int Server::_listenSocket(void)
{
	int result = listen(this->m_sockfd, 10);
	FD_SET(this->m_sockfd, &this->m_socketSet);
	return result;
}

int Server::_handleConnection(void)
{
	fd_set readfds;
	FD_ZERO(&readfds);

	for (;;) {
		readfds = this->m_socketSet;
		int fdMax = this->m_fdList.back() + 1;
		std::cout << fdMax << std::endl;
		struct timeval tv;
		tv.tv_sec = 10;
		tv.tv_usec = 0;
		if (select(FD_SETSIZE, &readfds, NULL, NULL, &tv) < 0) {
			this->_closeServer();
			exitWithError("Error: unable to select", 1);
		}

		for (int i = 0; i <= FD_SETSIZE; i++) {
			if (FD_ISSET(i, &readfds)) {
				printLog("Found Connection on fd " + i);
				if (i == this->m_sockfd) {
					if (this->_acceptConnection() < 0) {
						this->_closeServer();
						exitWithError("Error: unable to accept connection", 1);
					}
					this->_printPeerName();
					this->_printHostName();
				}
				else {
					if (this->_readSocket(i) <= 0) {
						std::cerr << "Error: unable to read from socket " << i << std::endl;
						close(i);
						this->m_fdList.erase(std::find(this->m_fdList.begin(), this->m_fdList.end(), i));
						FD_CLR(i, &this->m_socketSet);
						continue;
						// this->_closeServer();
						// exitWithError("Error: unable to read from socket", 1);
					}
					printLog("Message from client");
					std::cout << this->m_readBuffer;
					this->_generateResponse();
					if (this->_writeSocket(i) < 0) {
						this->_closeServer();
						exitWithError("Error: unable to write to socket", 1);
					}
					// this->m_fdList.erase(std::find(this->m_fdList.begin(), this->m_fdList.end(), this->m_acceptfd));
					// close(this->m_acceptfd);
				}
			}
		}
	}
	return 0;
}

int Server::_acceptConnection(void)
{
	socklen_t addr_size = sizeof(this->m_theirAddr);
	this->m_acceptfd = accept(this->m_sockfd, (struct sockaddr *)&this->m_theirAddr, &addr_size);
	if (this->m_acceptfd < 0)
		return -1;
	this->m_fdList.push_back(this->m_acceptfd);
	sort(this->m_fdList.begin(), this->m_fdList.end());
	FD_SET(this->m_acceptfd, &this->m_socketSet);
	printLog("Accepted connection on socket " + this->m_acceptfd);
	return this->m_acceptfd;
}

int Server::_readSocket(int sockfd)
{
	int bytesRead;
	bytesRead = recv(sockfd, this->m_readBuffer, 30000, 0);
	this->m_readBuffer[bytesRead] = '\0';
	return bytesRead;
}

int Server::_generateResponse(void)
{
	this->m_writeBuffer = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!\n";
	return 0;
}

int Server::_writeSocket(int sockfd)
{
	return send(sockfd, this->m_writeBuffer.c_str(), this->m_writeBuffer.length(), 0);
}

int Server::_printPeerName(void) {
	socklen_t addr_size = sizeof(this->m_theirAddr);
	getpeername(this->m_acceptfd, (struct sockaddr *)&this->m_theirAddr, &addr_size);
	std::cout << "Connection from: " << inet_ntoa(((struct sockaddr_in *)&this->m_theirAddr)->sin_addr) << std::endl;
	return 0;
}

int Server::_printHostName(void) {
	char hostname[1024];
	memset(hostname, 0, 1024);
	gethostname(hostname, 1023);
	std::cout << "Hostname: " << hostname << std::endl;
	return 0;
}

int Server::_closeServer(void)
{
	if (this->m_serverInfo != NULL)
		freeaddrinfo(this->m_serverInfo);
	for (int i = 0; i < (int) this->m_fdList.size(); i++) {
		if (this->m_fdList[i] > 2)
			close(this->m_fdList[i]);
	}
	// close(this->m_acceptfd);
	// close(this->m_sockfd);
	return 0;
}

void printLog(std::string message)
{
	if (LOG)
		std::cout << "========= " << message << " =========\n";
}
