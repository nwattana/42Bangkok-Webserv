#ifndef SERVER3_HPP
# define SERVER3_HPP

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
		int getClientMaxSize(void);
		int setupServer(void);
		int acceptConnection(void);
		void disconnectClient(int sockfd);
		int respond(int sockfd, std::string request, int statusCode=200);
		int closeServer(void);

	private:
		Server();
		std::string m_port;
		std::string m_ServerName;
		int m_clientMaxSize;
		struct addrinfo *m_serverInfo;

		RequestHandler *m_requestHandler;

		MySocketManager m_sockMan;
		MySocket *m_listener;

		std::string m_readBuffer;
		std::string m_writeBuffer;

		int _initServer(void);
		void _printAddressInfo(struct addrinfo *p);
		int _createSocket(void);
		int _bindAddress(void);
		int _listenSocket(void);
		int _generateResponse(int statusCode=200);
		int _writeSocket(int sockfd);
		int _printPeerName(MySocket *sock);
		int _printHostName(void);
};

void printLog(std::string message);

#endif
