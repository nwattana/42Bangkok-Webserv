#include "Server.hpp"

Server::Server()
{
	this->m_port = "8080";

	this->m_myAddr.sin_family = AF_INET;
	this->m_myAddr.sin_port = htons(atoi(this->m_port.c_str()));
	this->m_myAddr.sin_addr.s_addr = INADDR_ANY;
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
		exitWithError("Error: unable to setup server", 1);
	}
	this->_printAddressInfo();
	if (this->_setupServer() < 0) {
		exitWithError("Error: unable to setup server", 1);
	}
	if (this->_createServer() < 0) {
		exitWithError("Error: unable to create a socket", 1);
	}
	if (this->_bindServer() < 0) {
		// std::string errorMsg = "Error: unable to bind to port " + this->m_port;
		// exitWithError(errorMsg, 1);
		exitWithError();
	}
	if (this->_listenServer() < 0) {
		exitWithError("Error: unable to listen", 1);
	}
	if (this->_acceptServer() < 0) {
		exitWithError("Error: unable to accept connection", 1);
	}
	if (this->_readServer() < 0) {
		exitWithError("Error: unable to read from socket", 1);
	}
	else {
		std::cout << "====== Message from client ======" << "\n";
		std::cout << this->m_readBuffer;
	}
	//process message from server here
	this->_generateResponse();
	if (this->_writeServer() < 0) {
		exitWithError("Error: unable to write to socket", 1);
	}
	this->_printPeerName();
	this->_printHostName();
	if (this->_closeServer() < 0) {
		exitWithError("Error: unable to close acceptedSocket", 1);
	}
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

void Server::_printAddressInfo(void)
{
	char ipstr[INET6_ADDRSTRLEN];
	void *addr;
	std::string ipver;

	while (this->m_serverInfo != NULL)
	{

		// get the pointer to the address itself,
		// different fields in IPv4 and IPv6:
		if (this->m_serverInfo->ai_family == AF_INET) { // IPv4
			struct sockaddr_in *ipv4 = (struct sockaddr_in *)this->m_serverInfo->ai_addr;
			addr = &(ipv4->sin_addr);
			ipver="IPv4";
		}
		else { // IPv6
			struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)this->m_serverInfo->ai_addr;
			addr = &(ipv6->sin6_addr);
			ipver="IPv6";
		}

		// convert the IP to a string and print it:
		inet_ntop(this->m_serverInfo->ai_family, addr, ipstr, sizeof(ipstr));
		std::cout << "  " << ipver << ": " << ipstr << std::endl;

		// show content in struct
		std::cout << "  ai_family: " << this->m_serverInfo->ai_family << std::endl;
		std::cout << "  ai_socktype: " << this->m_serverInfo->ai_socktype << std::endl;
		std::cout << "  ai_protocol: " << this->m_serverInfo->ai_protocol << std::endl;
		std::cout << "  ai_addrlen: " << this->m_serverInfo->ai_addrlen << std::endl;
		// std::cout << "  ai_canonname: " << this->m_serverInfo->ai_canonname << std::endl;
		std::cout << "  ai_addr: " << this->m_serverInfo->ai_addr << std::endl;
		std::cout << "  ai_next: " << this->m_serverInfo->ai_next << std::endl;
		this->m_serverInfo = this->m_serverInfo->ai_next;
	}
}

int Server::_createServer(void)
{
	this->m_sockfd = socket(this->m_serverInfo->ai_family, this->m_serverInfo->ai_socktype, this->m_serverInfo->ai_protocol);
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
	close(this->m_acceptfd);
	close(this->m_sockfd);
	freeaddrinfo(this->m_serverInfo);
	return 0;
}
