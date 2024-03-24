#ifndef SERVERMANAGER_HPP
# define SERVERMANAGER_HPP

# include "Server.hpp"
# include "ConfigParser.hpp"
# include <bitset>

class ServerManager {
	public:
		ServerManager(ServerManager const &src);
		ServerManager &operator=(ServerManager const &rhs);
		~ServerManager();

		ServerManager(std::vector<ServerConfig> serverConfig); //initialize all servers
		/*Setting up all servers
		return 0 if success, -1 if fail*/
		int setupServer(void);
		int handleConnection(void);
		void closeAll(void);

	private:
		ServerManager();
		bool _checkAllConnection(fd_set *readfds, int *loopFd, int *loopServer);

		//utilities bundler
		void _tooglerw(int sockfd, bool isNowRead);
		int _readBuffer(int sockfd);
		void _disconnectClient(int sockfd);
		void _clearBufCache(void);
		int _readFrom(int sockfd);
		int _writeTo(int sockfd);

		std::vector<Server* >	m_servList;
		fd_set				m_readSockSet;
		fd_set				m_writeSockSet;
		std::string			m_buffer;
		int					m_totalBytesRead;
		bool				m_isReadLoop;
		std::map<int, int> m_sockStatCode;
		Server* m_currServ;
};

#endif
