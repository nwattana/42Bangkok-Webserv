#ifndef CFILE_HPP
#define CFILE_HPP

#include <unistd.h>
#include <vector>
#include <sys/socket.h>

class Cfile {
	public:
		Cfile();
		~Cfile();
		Cfile(Cfile const &src);
		Cfile &operator=(Cfile const &rhs);

		Cfile(int fd, bool isOpen, bool isSocket);
		int toggleOpen(bool toOpen=true);
		int getFd(void) const;
		bool isOpen(void) const;
		bool isSocket(void) const;
		int tryRecv(const char *buffer, size_t size, int flags);
		int trySend(const char *buffer, size_t size, int flags);

		bool operator>(Cfile const &rhs);
		bool operator>(int fd);
		bool operator<(Cfile const &rhs);
		bool operator<(int fd);
		bool operator==(Cfile const &rhs);
		bool operator==(int fd);
		bool operator!=(Cfile const &rhs);
		bool operator!=(int fd);
		bool operator>=(Cfile const &rhs);
		bool operator>=(int fd);
		bool operator<=(Cfile const &rhs);
		bool operator<=(int fd);
	private:
		int m_fd;
		bool m_isOpen;
		bool m_isSocket;
};

#endif
