#ifndef MYSOCKET_HPP
#define MYSOCKET_HPP

#include <unistd.h>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>

class MySocket {
	public:
		MySocket();
		~MySocket();
		MySocket(MySocket const &src);
		MySocket &operator=(MySocket const &rhs);

		MySocket(int fd, bool isConnection, bool isOpen=1);
		void setFd(int fd);
		int getFd(void) const;
		bool isOpen(void) const;
		bool isConnection(void) const;
		int tryRecv(const char *buffer, size_t size, int flags);
		int trySend(const char *buffer, size_t size, int flags);
		int closeFd(void);

		bool operator>(MySocket const &rhs);
		bool operator>(int fd);
		bool operator<(MySocket const &rhs);
		bool operator<(int fd);
		bool operator==(MySocket const &rhs);
		bool operator==(int fd);
		bool operator!=(MySocket const &rhs);
		bool operator!=(int fd);
		bool operator>=(MySocket const &rhs);
		bool operator>=(int fd);
		bool operator<=(MySocket const &rhs);
		bool operator<=(int fd);

		struct sockaddr_storage m_theirAddr;
		struct sockaddr_in m_myAddr;
		socklen_t m_addrSize;
	private:
		int m_fd;
		bool m_isOpen;
		bool m_isConnection;
};

#endif
