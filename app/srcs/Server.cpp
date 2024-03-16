#include "Server.hpp"

Server::Server()
{
	this->m_port = "8080";

	this->m_myAddr.sin_family = AF_INET;
	this->m_myAddr.sin_port = htons(atoi(this->m_port.c_str()));
	this->m_myAddr.sin_addr.s_addr = INADDR_ANY;

	this->m_serverInfo = NULL;
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
	this->_printAddressInfo(this->m_serverInfo);
	if (this->_setupServer() < 0) {
		this->_closeServer();
		exitWithError("Error: unable to setup server", 1);
	}
	if (this->_createServer() < 0) {
		this->_closeServer();
		exitWithError("Error: unable to create a socket", 1);
	}
	if (this->_bindServer() < 0) {
		this->_closeServer();
		// std::string errorMsg = "Error: unable to bind to port " + this->m_port;
		// exitWithError(errorMsg, 1);
		exitWithError();
	}
	if (this->_listenServer() < 0) {
		this->_closeServer();
		exitWithError("Error: unable to listen", 1);
	}
	if (this->_acceptServer() < 0) {
		this->_closeServer();
		exitWithError("Error: unable to accept connection", 1);
	}
	if (this->_readServer() < 0) {
		this->_closeServer();
		exitWithError("Error: unable to read from socket", 1);
	}
	else {
		std::cout << "====== Message from client ======" << "\n";
		std::cout << this->m_readBuffer;
	}
	//process message from server here
	this->_generateResponse();
	if (this->_writeServer() < 0) {
		this->_closeServer();
		exitWithError("Error: unable to write to socket", 1);
	}
	this->_printPeerName();
	this->_printHostName();
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

int Server::_createServer(void)
{
	this->m_sockfd = socket(this->m_serverInfo->ai_family, this->m_serverInfo->ai_socktype, this->m_serverInfo->ai_protocol);
	this->m_openedfd.push_back(this->m_sockfd);
	return this->m_sockfd;
}

int Server::_bindServer(void)
{
	return bind(this->m_sockfd, (struct sockaddr *)&this->m_myAddr, sizeof(this->m_myAddr));
}

int Server::_listenServer(void)
{
	return listen(this->m_sockfd, 10);
}

int Server::_acceptServer(void)
{
	socklen_t addr_size = sizeof(this->m_theirAddr);
	this->m_acceptfd = accept(this->m_sockfd, (struct sockaddr *)&this->m_theirAddr, &addr_size);
	this->m_openedfd.push_back(this->m_acceptfd);
	return this->m_acceptfd;
}

int Server::_readServer(void)
{
	int bytesRead;
	bytesRead = recv(this->m_acceptfd, this->m_readBuffer, 30000, 0);
	this->m_readBuffer[bytesRead] = '\0';
	return bytesRead;
}

int Server::_generateResponse(void)
{
	this->m_writeBuffer = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
	return 0;
}

int Server::_writeServer(void)
{
	return send(this->m_acceptfd, this->m_writeBuffer.c_str(), this->m_writeBuffer.length(), 0);
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
	for (int i = 0; i < this->m_openedfd.size(); i++) {
		if (this->m_openedfd[i] > 2)
			close(this->m_openedfd[i]);
	}
	// close(this->m_acceptfd);
	// close(this->m_sockfd);
	return 0;
}
