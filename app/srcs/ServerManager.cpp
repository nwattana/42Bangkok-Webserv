#include "../includes/ServerManager.hpp"

ServerManager::ServerManager()
{
	FD_ZERO(&this->m_socketSet);
}

ServerManager::ServerManager(std::vector<ServerConfig> serverConfig)
{
	FD_ZERO(&this->m_socketSet);
	for (size_t i = 0; i < serverConfig.size(); i++) {
		Server *server = new Server(serverConfig[i]);
		this->m_serverList.push_back(server);
	}
}

ServerManager::ServerManager(ServerManager const &src)
{
	*this = src;
}

ServerManager &ServerManager::operator=(ServerManager const &rhs)
{
	if (this != &rhs)
	{
		// copy
	}
	return *this;
}

ServerManager::~ServerManager()
{
	closeAll();
}

int ServerManager::setupServer(void)
{
	std::cout << this->m_serverList.size() << std::endl;
	for (size_t i = 0; i < this->m_serverList.size(); i++) {
		std::cout << "Setting up server " << i << std::endl;
		int fd = this->m_serverList[i]->setupServer();
		std::cout << "fd: " << fd << std::endl;
		if (fd < 0) {
			this->closeAll();
		}
		FD_SET(fd, &this->m_socketSet);
	}
	return (0);
}

int ServerManager::handleConnection(void)
{
	fd_set readfds;

	bool flag = false;
	int loopFd = 0, loopServer = 0;
	int totalBytesRead;
	std::string request;

	for (;;) {
		FD_ZERO(&readfds);
		if (!flag) {
			readfds = this->m_socketSet;
		}
		else {
			FD_SET(loopFd, &readfds);
		}
		struct timeval tv;
			tv.tv_sec = 10;
			tv.tv_usec = 0;
		if (select(FD_SETSIZE, &readfds, NULL, NULL, NULL) < 0) {
			this->closeAll();
			exitWithError("Error: unable to select", 1);
		}
		if (!flag) {
			flag = this->checkAllConnection(&readfds, &loopFd, &loopServer);
		}
		else {
			if (FD_ISSET(loopFd, &readfds)) {
				int bytesRead;
				char buffer[BUFFER_SIZE + 1];
				std::string cppbuffer;

				memset(buffer, 0, BUFFER_SIZE + 1);
				cppbuffer.clear();

				bytesRead = recv(loopFd, buffer, BUFFER_SIZE, 0);
				buffer[bytesRead] = '\0';
				// std::cout << "Bytes read: " << bytesRead << std::endl; std::cout << "Buffer: " << (int) buffer[0] << std::endl;
				if (bytesRead <= 0) {
					this->m_serverList[loopServer]->disconnectClient(loopFd);
					FD_CLR(loopFd, &this->m_socketSet);
					flag = false;
					totalBytesRead = 0;
					request.clear();
					continue;
				}
				cppbuffer = buffer;
				request += cppbuffer.substr(0, bytesRead);
				totalBytesRead += bytesRead;
				if (totalBytesRead >= this->m_serverList[loopServer]->getClientMaxSize()) {
					std::cerr << "Error: client message too large" << std::endl;
					this->m_serverList[loopServer]->respond(loopFd, request, 413);
					flag = false;
					totalBytesRead = 0;
					request.clear();
					continue;
				}
				if (request.find("\r\n") != std::string::npos) {
					request += '\0';
					std::cout << "Request:\n" << request << std::endl;
					this->m_serverList[loopServer]->respond(loopFd, request);
					flag = false;
					totalBytesRead = 0;
					request.clear();
					continue;
				}
			}


			// 	this->loopOneConnection(loopFd);
			// 	int res = this->m_serverList[j]->communicate(fd);
			// 	if (res > 0) {
			// 		FD_CLR(fd, &this->m_socketSet);
			// 	}
			// }
			else {
				flag = false;
				request += '\0';
				std::cout << "Request:\n" << request << std::endl;
				this->m_serverList[loopServer]->respond(loopFd, request);
				totalBytesRead = 0;
				request.clear();
			}
		}
	}
	return 0;
}

bool ServerManager::checkAllConnection(fd_set *readfds, int *loopFd, int *loopServer)
{
	for (int i = 0; i <= FD_SETSIZE; i++) {
		if (FD_ISSET(i, readfds)) {
			printLog("Found Connection on fd " + SSTR(i));
			for (int j = 0; j < this->m_serverList.size(); j++) {
				if (this->m_serverList[j]->getSocket(i) != NULL) {
					if (this->m_serverList[j]->getListenerFd() == i) {
						int newfd = this->m_serverList[j]->acceptConnection();
						if (newfd < 0) {
							this->m_serverList[j]->closeServer();
							exitWithError("Error: unable to accept connection", 1);
						}
						FD_SET(newfd, &this->m_socketSet);
					}
					else if (this->m_serverList[j]->getSocket(i) != NULL) {
						// int res = this->m_serverList[j]->communicate(i);
						// if (res > 0) {
						// 	FD_CLR(i, &this->m_socketSet);
						// }
						*loopFd = i;
						*loopServer = j;
						return true;
					}
					break;
				}
			}
		}
	}
	return false;
}

int ServerManager::loopOneConnection(int fd)
{
	return 0;
}

void ServerManager::closeAll(void) {
	for (int i = 0; i < this->m_serverList.size(); i++) {
		this->m_serverList[i]->closeServer();
	}
}
