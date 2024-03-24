#include "Server.hpp"

Server::Server() {
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
		//copy
	}
	return *this;
}

Server::Server(ServerConfig serverConfig) : m_serverConfig(serverConfig) {
	this->m_port = serverConfig.getConfig(S_LISTEN)[0];
	printLog("Port: " + this->m_port);
	this->m_ServerName = serverConfig.getConfig(S_SERVER_NAME)[0];
	printLog("Server Name: " + this->m_ServerName);
	this->m_clientMaxSize = atoi(serverConfig.getConfig(S_CLIENT_MAX_SIZE)[0].c_str());
	printLog("Client Max Size: " + SSTR(this->m_clientMaxSize));

	this->m_listener = NULL;
	this->m_serverInfo = NULL;
	this->m_requestHandler = NULL;
	this->m_isListening = false;
}

int Server::setupServer() {
	int res;
	//malloc things
	//setup listener
	this->m_listener = new MySocket();
	if (this->m_listener == NULL) {
		std::cerr << "Failed to allocate memory for listener" << std::endl;
		return -1;
	}
	this->m_listener->m_myAddr.sin_family = AF_INET;
	this->m_listener->m_myAddr.sin_port = htons(atoi(this->m_port.c_str()));
	this->m_listener->m_myAddr.sin_addr.s_addr = INADDR_ANY;
	//setup request handler
	m_requestHandler = new RequestHandler(this->m_serverConfig);
	if (m_requestHandler == NULL) {
		std::cerr << "Failed to allocate memory for request handler" << std::endl;
		return -1;
	}
	// get address info
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC; // use AF_INET6 to force IPv6
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // use my IP
	if (this->m_ServerName == "localhost")
		res = getaddrinfo(NULL, this->m_port.c_str(), &hints, &this->m_serverInfo);
	else
		res = getaddrinfo(this->m_ServerName.c_str(), this->m_port.c_str(), &hints, &this->m_serverInfo);
	if (res != 0) {
		std::cerr << "getaddrinfo() error: " << gai_strerror(res) << std::endl;
		return res;
	}
	// print address info
	this->_printAddressInfo(this->m_serverInfo);
	return 0;
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

int Server::setupSocket(void) {
	//create a socket
	int yes = 1;
	int sockfd = socket(this->m_serverInfo->ai_family, this->m_serverInfo->ai_socktype, this->m_serverInfo->ai_protocol);
	if (sockfd == -1) {
		std::cerr << "Failed to create socket" << std::endl;
		return -1;
	}
	this->m_listener->setFd(sockfd);
	this->m_sockMan.add(this->m_listener);
	fcntl(sockfd, F_SETFL, O_NONBLOCK);
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
	printLog("Socket created with fd " + SSTR(sockfd));
	// bind socket to address
	int res = bind(sockfd, this->m_serverInfo->ai_addr, this->m_serverInfo->ai_addrlen);
	if (res == -1) {
		std::cerr << "Failed to bind socket" << std::endl;
		return -1;
	}
	printLog("Socket binded to address");
	return (sockfd);
}

int Server::startListening(void) {
	int res = listen(this->m_listener->getFd(), 10);
	if (res == -1) {
		std::cerr << "Failed to listen" << std::endl;
		return -1;
	}
	printLog("Listening on port " + this->m_port);
	this->m_isListening = true;
	return 0;
}

int Server::acceptConnection(void) {
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

int Server::handleRequest(int sockfd, std::string request) {
	if (request.size() > this->m_clientMaxSize) {
		return this->_generateResponse(413);;
	}
	this->m_readBuffer[sockfd] = request;
	m_requestHandler->read_request(this->m_readBuffer[sockfd]);
	return this->_generateResponse(sockfd);
}

int Server::_generateResponse(int sockfd, int statusCode)
{
	if (statusCode == 200)
		this->m_writeBuffer[sockfd] = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!\n";
	else if (statusCode == 413)
		this->m_writeBuffer[sockfd] = "HTTP/1.1 413 Request Entity Too Large\nContent-Type: text/plain\nContent-Length: 31\n\nERROR Request entity too large!\n";
	else
		this->m_writeBuffer[sockfd] = "HTTP/1.1 500 Internal Server Error\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!\n";

	// this->m_writeBuffer = response.getResponse();
	return statusCode;
}

int Server::sendRespond(int sockfd) {
	int res = send(sockfd, this->m_writeBuffer[sockfd].c_str(), this->m_writeBuffer[sockfd].length(), 0);
	if (res != this->m_writeBuffer[sockfd].length()) {
		std::cerr << "Failed to send response" << std::endl;
		return -1;
	}
	return 0;
}

void Server::disconnectClient(int sockfd) {
	this->m_sockMan.remove(sockfd);
}

int Server::closeServer(void) {
	if (this->m_listener != NULL) {
		close(this->m_listener->getFd());
		delete this->m_listener;
	}
	if (this->m_serverInfo != NULL)
		freeaddrinfo(this->m_serverInfo);
	if (this->m_requestHandler != NULL)
		delete this->m_requestHandler;
	this->m_sockMan.closeAll();
	return 0;
}

int Server::getListenerFd(void) {
	return this->m_listener->getFd();
}

bool Server::isSocketValid(int sockfd) {
	if (this->getListenerFd() == sockfd)
		return true;
	return this->m_sockMan.hasSocket(sockfd);
}

int Server::getClientMaxSize(void) {
	return this->m_clientMaxSize;
}

bool Server::isListening(void) {
	return this->m_isListening;
}
