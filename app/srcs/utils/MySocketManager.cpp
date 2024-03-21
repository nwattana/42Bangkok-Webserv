#include "../../includes/MySocketManager.hpp"

MySocketManager::MySocketManager() {
}

MySocketManager::~MySocketManager() {
	this->closeAll();
}

MySocketManager::MySocketManager(MySocketManager const &src) {
	*this = src;
}

MySocketManager &MySocketManager::operator=(MySocketManager const &rhs) {
	if (this != &rhs) {
		this->m_socketList = rhs.m_socketList;
	}
	return *this;
}

MySocket* MySocketManager::get(int fd) {
	std::map<int, MySocket* >::iterator it = this->m_socketList.find(fd);
	if (it != this->m_socketList.end()) {
		return it->second;
	}
	return NULL;
}

void MySocketManager::add(MySocket* socketPtr) {
	this->m_socketList[socketPtr->getFd()] = socketPtr;
}

void MySocketManager::remove(int fd) {
	this->m_socketList[fd]->closeFd();
	delete this->m_socketList[fd];
	this->m_socketList.erase(fd);
}

int MySocketManager::closeAll(void) {
	int ret = 0;
	std::map<int, MySocket* >::iterator it = this->m_socketList.begin();
	while (it != this->m_socketList.end()) {
		if (it->second->closeFd() < 0) {
			ret = -1;
		}
		delete it->second;
		it++;
	}
	return ret;
}

int MySocketManager::tryRecv(int fd, const char *buffer, size_t size, int flags) {
	return this->m_socketList[fd]->tryRecv(buffer, size, flags);
}

int MySocketManager::trySend(int fd, const char *buffer, size_t size, int flags) {
	return this->m_socketList[fd]->trySend(buffer, size, flags);
}

int MySocketManager::getMaxFd(void) {
	int max = 0;
	std::map<int, MySocket* >::iterator it = this->m_socketList.begin();
	while (it != this->m_socketList.end())
	{
		if (it->first > max)
			max = it->first;
		it++;
	}
	return max;
}
