#ifndef SERVER_HPP
# define SERVER_HPP

# include "ServerConfig.hpp"
# include "MySocketManager.hpp"
# include "utils.hpp"
# include <fcntl.h>
# include <sys/select.h>
# include <sys/time.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <algorithm>
# include <string> // Include the <string> header
# include "RequestHandler.hpp"

# ifndef LOG
#  define LOG 0
# endif

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 30000
#endif

class Server {
	public:
		Server(ServerConfig serverConfig);
		~Server();
		Server(Server const &src);
		Server &operator=(Server const &rhs);

		const MySocket* getSocket(int fd);
		int getListenerFd(void);
		int setupServer(void);
		int acceptConnection(void);
		int communicate(int sockfd);
		int closeServer(void);

	private:
		Server();
		std::string m_port;
		std::string m_ServerName;
		struct addrinfo *m_serverInfo;

		RequestHandler *m_requestHandler;

		MySocketManager m_sockMan;
		MySocket *m_listener;
		// int m_sockfd;
		// struct sockaddr_in m_myAddr;
		// int m_acceptfd;
		// struct sockaddr_storage m_theirAddr;
		// fd_set m_socketSet; //removing
		// CfileManager m_cfileMan;

		char m_readBuffer[BUFFER_SIZE + 1];
		std::string m_writeBuffer;

		int _initServer(void);
		void _printAddressInfo(struct addrinfo *p);
		int _createSocket(void);
		int _bindAddress(void);
		int _listenSocket(void);
		int _handleConnection(void);
		int _readSocket(int sockfd);
		int _generateResponse(void);
		int _writeSocket(int sockfd);
		int _printPeerName(MySocket *sock);
		int _printHostName(void);
};

void printLog(std::string message);

#endif
