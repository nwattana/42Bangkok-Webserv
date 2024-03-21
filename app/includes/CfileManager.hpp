#ifndef CFILEMANAGER_HPP
#define CFILEMANAGER_HPP

#include "Cfile.hpp"
#include <map>
#include <vector>
#include <algorithm>

class CfileManager{
	public:
		CfileManager();
		~CfileManager();
		CfileManager(CfileManager const &src);
		CfileManager &operator=(CfileManager const &rhs);

		Cfile get(int fd);
		void add(int fd, bool isOpen, bool isSocket);
		void remove(int fd);
		int toggleOpen(int fd, bool toOpen=true);
		int closeAll(void);
		int tryRecv(int fd, const char *buffer, size_t size, int flags);
		int trySend(int fd, const char *buffer, size_t size, int flags);
		int getMaxFd(void);

	private:
		std::map<int, Cfile> m_cfileList;
};

#endif
