#ifndef CFILEMANAGER_HPP
#define CFILEMANAGER_HPP

#include "MySocket.hpp"
#include <map>
#include <vector>
#include <algorithm>
#include <iostream>

class MySocketManager{
	public:
		MySocketManager();
		~MySocketManager();
		MySocketManager(MySocketManager const &src);
		MySocketManager &operator=(MySocketManager const &rhs);

		MySocket* get(int fd);
		void add(MySocket* socketPtr);
		void remove(int fd);
		int closeAll(void);
		int tryRecv(int fd, const char *buffer, size_t size, int flags);
		int trySend(int fd, const char *buffer, size_t size, int flags);
		int getMaxFd(void);
		bool hasSocket(int fd);

	private:
		std::map<int, MySocket* > m_socketList;
};

#endif
