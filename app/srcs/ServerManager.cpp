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
	std::cout << this->m_servList.size() << std::endl;
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
	fd_set readfds, writefds;

	this->m_isReadLoop = false;
	int loopFd = 0;
	struct timeval tv;

	for (;;) {
		FD_ZERO(&readfds);
		FD_ZERO(&writefds);
		if (this->m_isReadLoop) {
			FD_SET(loopFd, &readfds);
			tv.tv_sec = 1;
			tv.tv_usec = 0;
		}
		else {
			readfds = this->m_readSockSet;
			writefds = this->m_writeSockSet;
			tv.tv_sec = 10;
			tv.tv_usec = 0;
		}
		int res = select(FD_SETSIZE, &readfds, &writefds, NULL, &tv);
		if (res < 0) {
			std::cerr << "Error: select failed" << std::endl;
			return -1;
		}
		if (this->m_isReadLoop) {
			if (res > 0 && FD_ISSET(loopFd, &readfds)) {
				this->_readBuffer(loopFd);
			}
			else {
				this->m_isReadLoop = false;
				this->_tooglerw(loopFd, true);
				continue;
			}
		}
		else {
			for (int i = 0; i <= FD_SETSIZE; i++) {
				if (FD_ISSET(i, &readfds)) {
					this->_readFrom(i);
				}
				if (FD_ISSET(i, &writefds)) {
					this->_writeTo(i);
				}
			}
		}
	}
	return (0);
}

void ServerManager::closeAll(void) {
	for (std::vector<Server*>::iterator it = this->m_servList.begin(); it != this->m_servList.end(); it++) {
		delete *it;
	}
	FD_ZERO(&this->m_readSockSet);
	FD_ZERO(&this->m_writeSockSet);
	this->m_servList.clear();
}

void ServerManager::_tooglerw(int sockfd, bool isNowRead) {
	if (isNowRead) {
		FD_CLR(sockfd, &this->m_readSockSet);
		FD_SET(sockfd, &this->m_writeSockSet);
	}
	else {
		FD_CLR(sockfd, &this->m_writeSockSet);
		FD_SET(sockfd, &this->m_readSockSet);
		this->_clearBufCache();
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
	if (bytesRead <= 0) {
		this->_disconnectClient(sockfd);
		return bytesRead;
	}

	cppbuffer = buffer;
	this->m_buffer += cppbuffer.substr(0, bytesRead);
	this->m_totalBytesRead += bytesRead;
	if (this->m_totalBytesRead > this->m_currServ->getClientMaxSize()) {
		m_sockStatCode[sockfd] = this->m_currServ->handleRequest(sockfd, this->m_buffer);
		this->_tooglerw(sockfd, true);
	}
	if (this->m_buffer.find("\r\n\r\n") != std::string::npos) {
		this->m_buffer += '\0';
		std::cout << "Request:\n" << this->m_buffer << std::endl;
		m_sockStatCode[sockfd] = this->m_currServ->handleRequest(sockfd, this->m_buffer);
		this->_tooglerw(sockfd, true);
	}
}

void ServerManager::_disconnectClient(int sockfd) {
	m_currServ->disconnectClient(sockfd);
	FD_CLR(sockfd, &this->m_readSockSet);
	this->m_isReadLoop = false;
	this->_clearBufCache();
}

void ServerManager::_clearBufCache(void) {
	this->m_buffer.clear();
	this->m_totalBytesRead = 0;
}

int ServerManager::_readFrom(int sockfd) {
	for (std::vector<Server*>::iterator it = this->m_servList.begin(); it != this->m_servList.end(); it++) {
		if ((*it)->getListenerFd() == sockfd) {
			this->m_currServ = *it;
		}
		else {
			continue;
		}
		if (this->m_currServ->getListenerFd() == sockfd) {
			if (!this->m_currServ->isListening()) {
				this->m_currServ->startListening();
			}
			else {
				int newSock = this->m_currServ->acceptConnection();
				if (newSock < 0) {
					std::cerr << "Error: failed to accept connection" << std::endl;
					return -1;
				}
				FD_SET(newSock, &this->m_readSockSet);
			}
		}
		else {
			this->m_isReadLoop = true;
			this->_tooglerw(sockfd, false);
		}
	}
	return -1;
}

int ServerManager::_allowRespond(int sockfd) {
	if (m_sockStatCode[sockfd] == 200) {
		m_currServ->sendRespond(sockfd);
	}
	else {
		m_currServ->sendRespond(sockfd);
	}
	FD_CLR(sockfd, &this->m_writeSockSet);
}
