#ifndef SERVER_HPP
# define SERVER_HPP

# include "ServerConfig.hpp"
# include "MySocketManager.hpp"
# include "utils.hpp"
# include "RequestHandler.hpp"
# include <fcntl.h>
# include <sys/select.h>
# include <sys/time.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <algorithm>
# include <string>

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 30000
#endif

class Server {
	public:
		Server(ServerConfig serverConfig);
		~Server();
		Server(Server const &src);
		Server &operator=(Server const &rhs);

		/* setup server according to cofigurations (serverConfig) sepcified in constructor
		return 0 if success, nonzero if fail */
		int	setupServer();
		/* create socket for server and bind it to the specified port
		return sockfd if success, -1 if fail */
		int	setupSocket(void);
		/* allows the listener to be listening
		return 0 if success, -1 if fail */
		int startListening(void);
		/* accept connection from client
		return sockfd if success, -1 if fail */
		int	acceptConnection(void);
		/* generate response to client based from request
		return request code if successfully handle the request, -1 if fail */
		int	handleRequest(int sockfd, std::string request);
		/* send response to client
		return 0 if success, -1 if fail */
		int	sendRespond(int sockfd);
		void disconnectClient(int sockfd);
		/* close server
		return 0 if success, -1 if all servers needs to be closed */
		int	closeServer(void);

		int		getListenerFd(void);
		bool	isSocketValid(int sockfd);
		int		getClientMaxSize(void);
		bool	isListening(void);

	private:
		Server();
		ServerConfig m_serverConfig; //server configurations
		std::string m_port; //server configurations
		std::string m_ServerName; //server configurations
		int m_clientMaxSize; //server configurations

		struct addrinfo *m_serverInfo; //address info of server
		MySocket *m_listener; // listening socket
		MySocketManager m_sockMan; //all connection sockets

		RequestHandler *m_requestHandler;

		void _printAddressInfo(struct addrinfo *p);
		int _printPeerName(MySocket *sock);
		int _printHostName(void);
		int _generateResponse(int statusCode=200);

		std::string m_readBuffer;
		std::string m_writeBuffer;
		bool m_isListening;
};

#endif
