#include "ServerManager.hpp"

ServerManager::ServerManager() {
}

ServerManager::~ServerManager() {
	this->closeAll();
}

ServerManager::ServerManager(ServerManager const &src) {
	*this = src;
}

ServerManager &ServerManager::operator=(ServerManager const &rhs) {
	if (this != &rhs) {
		// copy
	}
	return *this;
}

ServerManager::ServerManager(std::vector<ServerConfig> serverConfig) {
	FD_ZERO(&this->m_readSockSet);
	FD_ZERO(&this->m_writeSockSet);
	for (size_t i = 0; i < serverConfig.size(); i++) {
		Server *server = new Server(serverConfig[i]);
		this->m_servList.push_back(server);
	}
}

int ServerManager::setupServer(void) {
	db_red("SERVER LIST SIZE : " + std::string(YELLOW) + SSTR(this->m_servList.size()));
	for (std::vector<Server*>::iterator it = this->m_servList.begin(); it != this->m_servList.end(); it++) {
		int i = it - this->m_servList.begin();
		printLog("Setting up server " + SSTR(i) + "\n");
		int res = (*it)->setupServer();
		if (res < 0) {
			this->closeAll();
			return -1;
		}
		printLog("setup successful");
	}
	for (std::vector<Server*>::iterator it = this->m_servList.begin(); it != this->m_servList.end(); it++) {
		int i = it - this->m_servList.begin();
		printLog("Setting up socket for server " + SSTR(i) + "\n");
		int fd = (*it)->setupSocket();
		if (fd < 0) {
			this->closeAll();
			return -1;
		}
		FD_SET(fd, &this->m_readSockSet);
	}
	return (0);
}

int ServerManager::handleConnection(void) {
	std::cout << CYAN << "Server ready to accept connections" << std::endl;
	fd_set readfds, writefds, errorfds;

	this->m_isReadLoop = false;
	int loopFd = 0;
	struct timeval tv;

	for (;;) {
		FD_ZERO(&readfds);
		FD_ZERO(&writefds);
		FD_ZERO(&errorfds);

		if (this->m_isReadLoop) {
			FD_SET(loopFd, &readfds);
			tv.tv_sec = 0;
			tv.tv_usec = 10000;
		}
		else {
			readfds = this->m_readSockSet;
			writefds = this->m_writeSockSet;
			tv.tv_sec = 10;
			tv.tv_usec = 0;
		}


		int res = select(FD_SETSIZE, &readfds, &writefds, &errorfds, &tv);
		if (res < 0) {
			db_red("select failed is_read_loop = " + SSTR(this->m_isReadLoop));
			db_red("Loop fd = " + SSTR(loopFd));
			perror("select failed");
			recovery_socket_improper_close();
			loopFd = 0;
			// function check invalid socket
			continue;
		}
		if (this->m_isReadLoop) {
			if (res > 0 && FD_ISSET(loopFd, &readfds)) {
				this->_readBuffer(loopFd);
			}
			else {
				this->m_isReadLoop = false;
				this->_toggle_rw(loopFd, true);
				continue;
			}
		}
		else {
			for (int i = 0; i <= FD_SETSIZE; i++) {
				if (FD_ISSET(i, &readfds)) {
					printLog("Reading from " + SSTR(i));
					res = this->_readFrom(i);
					if (res == 1) {
						loopFd = i;
						break;
					}
				}
				else if (FD_ISSET(i, &writefds)) {
					printLog("Writing to " + SSTR(i));
					this->_writeTo(i);
				}
			}
		}
	}
	return (0);
}

void ServerManager::recovery_socket_improper_close()
{
	int try_recv = 0;
	char buff[1];
	// loop through all servers

	db_red(std::string(GREEN) + "Enter RECovery");
	for (std::vector<Server*>::iterator it = this->m_servList.begin(); it != this->m_servList.end(); it++) {
		// loop through all sockets connected to the server
		std::map<int, MySocket* >* socketList = (*it)->m_sockMan.getSocketList();
		if (socketList->size() == 1)
		{
			continue;
		}
		db_red(std::string(GREEN) + "START SERVER : " + std::string((*it)->getServerName()) + ", Connection size : " + SSTR(socketList->size()));
		for (std::map<int, MySocket* >::iterator it2 = socketList->begin(); it2 != socketList->end(); it2++) {
			// try to recv 1 byte from the socket
			try_recv = recv(it2->first, buff, 1, MSG_PEEK);
			// if recv failed, close the socket
			if (try_recv <= 0) {
				if (errno)
				{
					perror("what : ");
					exit(1);
				}
				db_red("DEBUG: 2");
				this->_toggle_rw(it2->first, TO_READ);
				db_red("DEBUG: 3");
				FD_CLR(it2->first, &this->m_readSockSet);
				db_red("DEBUG: 4");
				
				// close meth MySocket
				// it2->second->closeFd();

				// remove metho MySocketMenager
				// (*it)->m_sockMan.remove(it2->first);
				db_green("remove socket " + SSTR(it2->first) + " from read set");
				db_red("DEBUG: 5");
				_disconnectClient(it2->first);
				db_red("DEBUG: 6");
			}
		}
		db_red(std::string(GREEN) + "FINISH SERVER : " + std::string((*it)->getServerName()));
	}
	db_red("EXIT_RECOVERY");
}

void ServerManager::closeAll(void) {
	// for (std::vector<Server*>::iterator it = this->m_servList.begin(); it != this->m_servList.end(); it++) {
	// 	delete *it;
	// }
	this->m_servList.clear();
	FD_ZERO(&this->m_readSockSet);
	FD_ZERO(&this->m_writeSockSet);
}

/// @brief toggle sockfd read/write
void ServerManager::_toggle_rw(int sockfd, bool isNowRead) {
	if (isNowRead) {
		// remove from read set
		if (FD_ISSET(sockfd, &this->m_readSockSet))
			FD_CLR(sockfd, &this->m_readSockSet);
		FD_SET(sockfd, &this->m_writeSockSet);
	}
	else {
		// remove from write set
		if (FD_ISSET(sockfd, &this->m_writeSockSet))
			FD_CLR(sockfd, &this->m_writeSockSet);
		FD_SET(sockfd, &this->m_readSockSet);
		this->_clearBufCache(sockfd);
	}
}

int ServerManager::_readBuffer(int sockfd) {
	int bytesRead;
	char buffer[BUFFER_SIZE + 1];
	std::string cppbuffer;

	memset(buffer, 0, BUFFER_SIZE + 1);
	cppbuffer.clear();

	bytesRead = recv(sockfd, buffer, BUFFER_SIZE, 0);
	buffer[bytesRead] = '\0';
	printLog("Bytes read: " + SSTR(bytesRead));
	printLog("\n" + std::string(buffer) + "\n <- Bytes read: from buff");
	if (bytesRead <= 0) {
		db_red("Error: failed to read from socket");
		this->_disconnectClient(sockfd);
		return bytesRead;
	}

	db_red("DEBUG: 1");

	cppbuffer = buffer;
	// ต่อ string ที่อ่านได้เข้ากับ string ที่อ่านไว้ก่อนหน้า
	this->m_bufCache[sockfd] += cppbuffer.substr(0, bytesRead);
	// บวกจำนวน byte ที่อ่านได้เข้ากับจำนวน byte ที่อ่านไว้ก่อนหน้า
	this->m_totalBytesRead[sockfd] += bytesRead;

	// check if the request is too big
	if (this->m_totalBytesRead[sockfd] > this->m_currServ->getClientMaxSize()) {
		m_sockStatCode[sockfd] = this->m_currServ->handleRequest(sockfd, this->m_bufCache[sockfd]);
		this->_toggle_rw(sockfd, true);
	}

	// check if the request is complete
	if (this->m_bufCache[sockfd].find("\r\n") != std::string::npos) {
		this->m_bufCache[sockfd].append("\0");
		this->m_totalBytesRead[sockfd]++;
		// std::cout << "Request:\n" << this->m_bufCache[sockfd] << std::endl;
		// for (int i = 0; i < this->m_bufCache[sockfd].size(); i++) {
		// 	std::cout << (int) this->m_bufCache[sockfd][i] << " ";
		// }
		// std::cout << std::endl;
		m_sockStatCode[sockfd] = this->m_currServ->handleRequest(sockfd, this->m_bufCache[sockfd]);
		this->_toggle_rw(sockfd, TO_WRITE);
	}

	return bytesRead;
}

void ServerManager::_disconnectClient(int sockfd) {
	m_currServ->disconnectClient(sockfd);
	FD_CLR(sockfd, &this->m_readSockSet);
	this->m_isReadLoop = false;
	this->_clearBufCache(sockfd);
}

void ServerManager::_clearBufCache(int sockfd) {
	try {
		// check sockfd is in map
		if (this->m_bufCache.find(sockfd) != this->m_bufCache.end()) {
			this->m_bufCache[sockfd].clear();
		}
		// check sockfd is in map
		if (this->m_totalBytesRead.find(sockfd) != this->m_totalBytesRead.end()) {
			this->m_totalBytesRead[sockfd] = 0;
		}
	}
	catch (const std::exception &e) {
		std::cerr << "Error: failed to clear buffer cache" << std::endl;
	}
}

int ServerManager::_readFrom(int sockfd) {
	for (std::vector<Server*>::iterator it = this->m_servList.begin(); it != this->m_servList.end(); it++) {
		if ((*it)->isSocketValid(sockfd)) {
			this->m_currServ = *it;
			printLog("Reading from server #" + SSTR(it - this->m_servList.begin()));
		}
		else {
			continue;
		}
		if (this->m_currServ->getListenerFd() == sockfd) {
			if (!this->m_currServ->isListening()) {
				this->m_currServ->startListening();
				printLog("The server now started listening");
				return 0;
			}
			else {
				int newSock = this->m_currServ->acceptConnection();
				if (newSock < 0) {
					std::cerr << "Error: failed to accept connection" << std::endl;
					return -1;
				}
				FD_SET(newSock, &this->m_readSockSet);
				printLog("Accepted connection on socket " + SSTR(newSock));
				return 0;
			}
		}
		else {
			printLog("Reading from client " + SSTR(sockfd));
			this->m_isReadLoop = true;
			this->_toggle_rw(sockfd, false);
			return 1;
		}
	}
	return -1;
}

int ServerManager::_writeTo(int sockfd) {
	for (std::vector<Server*>::iterator it = this->m_servList.begin(); it != this->m_servList.end(); it++) {
		if ((*it)->isSocketValid(sockfd)) {
			this->m_currServ = *it;
		}
		else {
			continue;
		}
		int res = m_currServ->sendRespond(sockfd);
		if (res < 0) {
			std::cerr << "Error: failed to send response" << std::endl;
			this->_disconnectClient(sockfd);
		}
		this->_toggle_rw(sockfd, TO_READ);
		printLog("Sent response to client " + SSTR(sockfd));
		return 0;
	}
	return -1;
}
