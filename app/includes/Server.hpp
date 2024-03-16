#ifndef SERVER_HPP
#define SERVER_HPP

#include "Webserv.hpp"
#include <arpa/inet.h>

class Server {
	public:
		Server();
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
		char m_readBuffer[30001];
		std::string m_writeBuffer;

		int _setupServer(void);
		void _printAddressInfo(void);
		int _createServer(void);
		int _bindServer(void);
		int _listenServer(void);
		int _acceptServer(void);
		int _readServer(void);
		int _generateResponse(void);
		int _writeServer(void);
		int _printPeerName(void);
		int _printHostName(void);
		int _closeServer(void);
};

#endif
