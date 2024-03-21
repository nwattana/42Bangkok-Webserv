#include "../includes/Server.hpp"

Server::Server() {
	this->m_port = "8080";
	this->m_ServerName = "localhost";

	this->m_listener = new MySocket();
	this->m_listener->m_myAddr.sin_family = AF_INET;
	this->m_listener->m_myAddr.sin_port = htons(atoi(this->m_port.c_str()));
	this->m_listener->m_myAddr.sin_addr.s_addr = INADDR_ANY;

	this->m_serverInfo = NULL;

}

Server::Server(ServerConfig serverConfig) {
	this->m_port = serverConfig.getConfig(S_LISTEN)[0];
	std::cout << "Port: " << this->m_port << std::endl;
	this->m_ServerName = serverConfig.getConfig(S_SERVER_NAME)[0];
	std::cout << "Server Name: " << this->m_ServerName.c_str() << std::endl;

	this->m_listener = new MySocket();
	this->m_listener->m_myAddr.sin_family = AF_INET;
	this->m_listener->m_myAddr.sin_port = htons(atoi(this->m_port.c_str()));
	this->m_listener->m_myAddr.sin_addr.s_addr = INADDR_ANY;

	// this->m_serverInfo = NULL;
}

Server::~Server() {
	this->closeServer();
}

Server::Server(Server const &src)
{
	*this = src;
}

Server &Server::operator=(Server const &rhs) {
	if (this != &rhs) {
		// copy
	}
	return *this;
}

const MySocket* Server::getSocket(int fd) {
	return this->m_sockMan.get(fd);
}

int Server::getListenerFd(void) {
	return this->m_listener->getFd();
}

int Server::setupServer(void)
{
	std::cout << "Setting up serverrr" << std::endl;
	int res;
	res = this->_initServer();
	std::cout << "Server setup successfully" << std::endl;
	if (res < 0) {

		this->closeServer();
		// exitWithError("Error: unable to setup server", 1);
		exitWithError(gai_strerror(res), 1);
	}
	printLog("Server setup successfully");
	this->_printAddressInfo(this->m_serverInfo);
	if (this->_createSocket() < 0) {
		this->closeServer();
		exitWithError("Error: unable to create a socket", 1);
	}
	printLog("Socket created successfully");
	if (this->_bindAddress() < 0) {
		this->closeServer();
		// std::string errorMsg = "Error: unable to bind to port " + this->m_port;
		// exitWithError(errorMsg, 1);
		exitWithError();
	}
	printLog("Binded to port " + this->m_port);
	if (this->_listenSocket() < 0) {
		this->closeServer();
		exitWithError("Error: unable to listen", 1);
	}
	printLog("Listening on port " + this->m_port);
	std::cout << "fd: " << this->m_listener->getFd() << std::endl;
	return this->m_listener->getFd();
}

int Server::_initServer(void)
{
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC; // use AF_INET6 to force IPv6
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // use my IP
	std::cout << "Server Name: " << this->m_ServerName << std::endl;
	std::cout << "Port: " << this->m_port.c_str() << std::endl;
	// return getaddrinfo(serverName, this->m_port.c_str(), &hints, &this->m_serverInfo);
	if (this->m_ServerName == "localhost")
		return getaddrinfo(NULL, this->m_port.c_str(), &hints, &this->m_serverInfo);
	else
		return getaddrinfo(this->m_ServerName.c_str(), this->m_port.c_str(), &hints, &this->m_serverInfo);
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

int Server::_createSocket(void)
{
	int yes = 1;
	int sockfd = socket(this->m_serverInfo->ai_family, this->m_serverInfo->ai_socktype, this->m_serverInfo->ai_protocol);
	this->m_listener->setFd(sockfd);
	this->m_sockMan.add(this->m_listener);
	fcntl(sockfd, F_SETFL, O_NONBLOCK);
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
	printLog("Socket created with fd " + SSTR(sockfd));
	return sockfd;
}

int Server::_bindAddress(void)
{
	std::cout << "Binding to fd " << this->m_listener->getFd() << std::endl;
	// return bind(this->m_sockfd, (struct sockaddr *)&this->m_myAddr, sizeof(this->m_myAddr));
	return bind(this->m_listener->getFd(), this->m_serverInfo->ai_addr, this->m_serverInfo->ai_addrlen);
}

int Server::_listenSocket(void)
{
	int result = listen(this->m_listener->getFd(), 10);
	return result;
}

int Server::acceptConnection(void)
{
	MySocket *newSocket = new MySocket(-1, true);
	socklen_t addr_size = sizeof(newSocket->m_theirAddr);
	int newFd = accept(this->m_listener->getFd(), (struct sockaddr *)&newSocket->m_theirAddr, &addr_size);
	newSocket->setFd(newFd);
	if (newFd < 0)
		return newFd;
	this->m_sockMan.add(newSocket);
	printLog("Accepted connection on socket " + SSTR(newFd));
	this->_printPeerName(newSocket);
	this->_printHostName();
	return newFd;
}

int Server::communicate(int sockfd)
{
	if (this->_readSocket(sockfd) <= 0) {
		std::cerr << "Error: unable to read from socket " << sockfd << std::endl;
		this->m_sockMan.remove(sockfd);
		return sockfd;
	}
	printLog("Message from client");
	std::cout << this->m_readBuffer;
	this->_generateResponse();
	if (this->_writeSocket(sockfd) < 0) {
		std::cerr << "Error: unable to write to socket " << sockfd << std::endl;
		this->m_sockMan.remove(sockfd);
		return sockfd;
	}
	printLog("Response sent to client");
	return 0;
}

int Server::_readSocket(int sockfd)
{
	int bytesRead;
	bytesRead = recv(sockfd, this->m_readBuffer, BUFFER_SIZE, 0);
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

int Server::_printPeerName(MySocket *sock) {
	socklen_t addr_size = sizeof(sock->m_theirAddr);
	getpeername(sock->getFd(), (struct sockaddr *)&sock->m_theirAddr, &addr_size);
	std::cout << "Connection from: " << inet_ntoa(((struct sockaddr_in *)&sock->m_theirAddr)->sin_addr) << std::endl;
	return 0;
}

int Server::_printHostName(void) {
	char hostname[1024];
	memset(hostname, 0, 1024);
	gethostname(hostname, 1023);
	std::cout << "Hostname: " << hostname << std::endl;
	return 0;
}

int Server::closeServer(void)
{
	if (this->m_serverInfo != NULL)
		freeaddrinfo(this->m_serverInfo);
	this->m_sockMan.closeAll();
	// close(this->m_acceptfd);
	// close(this->m_sockfd);
	return 0;
}

void printLog(std::string message)
{
	if (LOG)
		std::cout << "========= " << message << " =========\n";
}
