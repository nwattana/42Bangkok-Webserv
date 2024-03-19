#pragma once
#ifndef SERVER_HPP
# define SERVER_HPP

# include "Webserv.hpp"
# include <fcntl.h>
# include <sys/select.h>
# include <sys/time.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <algorithm>
# include "ServerConfig.hpp"

# ifndef LOG
#  define LOG 0
# endif

class Server {
	public:
		Server();
		Server(ServerConfig serverConfig);
		~Server();
		Server(Server const &src);
		Server &operator=(Server const &rhs);

		int connectServer(void);
	private:
		int m_sockfd;
		int m_acceptfd;
		std::string m_port;
		struct addrinfo *m_serverInfo;
		struct sockaddr_in m_myAddr;
		struct sockaddr_storage m_theirAddr;
		fd_set m_socketSet;
		char m_readBuffer[30001];
		std::string m_writeBuffer;
		std::vector<int> m_fdList;

		int _setupServer(void);
		void _printAddressInfo(struct addrinfo *p);
		int _createSocket(void);
		int _bindAddress(void);
		int _listenSocket(void);
		int _handleConnection(void);
		int _acceptConnection(void);
		int _readSocket(int sockfd);
		int _generateResponse(void);
		int _writeSocket(int sockfd);
		int _printPeerName(void);
		int _printHostName(void);
		int _closeServer(void);
};

void printLog(std::string message);

#endif
