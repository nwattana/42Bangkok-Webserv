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
	FD_ZERO(&readfds);

	for (;;) {
		readfds = this->m_socketSet;
		struct timeval tv;
		tv.tv_sec = 10;
		tv.tv_usec = 0;
		if (select(FD_SETSIZE, &readfds, NULL, NULL, &tv) < 0) {
			this->closeAll();
			exitWithError("Error: unable to select", 1);
		}

		for (int i = 0; i <= FD_SETSIZE; i++) {
			if (FD_ISSET(i, &readfds)) {
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
							int res = this->m_serverList[j]->communicate(i);
							if (res > 0) {
								// it->closeServer();
								FD_CLR(i, &this->m_socketSet);
							}
						}
						break;
					}
				}
			}
		}
	}
	return 0;
}

void ServerManager::closeAll(void) {
	for (int i = 0; i < this->m_serverList.size(); i++) {
		this->m_serverList[i]->closeServer();
	}
}
