#ifndef SERVERMANAGER3_HPP
# define SERVERMANAGER3_HPP

# include "Server.hpp"
# include "ConfigParser.hpp"

class ServerManager {
	public:
		ServerManager(ServerManager const &src);
		ServerManager &operator=(ServerManager const &rhs);
		~ServerManager();

		ServerManager(std::vector<ServerConfig> serverConfig); //initialize all servers
		int setupServer(void); //set up servers to get sockets
		int handleConnection(void); //handle connection using listen()
		void closeAll(void);

	private:
		ServerManager();
		bool checkAllConnection(fd_set *readfds, int *loopFd, int *loopServer);

		std::vector<Server* >	m_serverList;
		fd_set				m_socketSet;
};

#endif
