#include "../../includes/MySocket.hpp"

MySocket::MySocket() : m_fd(-1), m_isOpen(false), m_isConnection(false), m_addrSize(sizeof(this->m_theirAddr)) {
}

MySocket::MySocket(int fd, bool isConnection, bool isOpen) : m_fd(fd), m_isOpen(isOpen), m_isConnection(isConnection), m_addrSize(sizeof(this->m_theirAddr)) {
}

MySocket::~MySocket() {
	if (this->m_isOpen) {
		this->closeFd();
	}
}

MySocket::MySocket(MySocket const &src) {
	*this = src;
}

MySocket &MySocket::operator=(MySocket const &rhs) {
	if (this != &rhs) {
		this->m_fd = rhs.m_fd;
		this->m_isOpen = rhs.m_isOpen;
		this->m_isConnection = rhs.m_isConnection;
		this->m_theirAddr = rhs.m_theirAddr;
		this->m_myAddr = rhs.m_myAddr;
		this->m_addrSize = rhs.m_addrSize;
	}
	return *this;
}

void MySocket::setFd(int fd) {
	this->m_fd = fd;
	this->m_isOpen = true;
}

int MySocket::getFd(void) const {
	return this->m_fd;
}

bool MySocket::isOpen(void) const {
	return this->m_isOpen;
}

bool MySocket::isConnection(void) const {
	return this->m_isConnection;
}

int MySocket::tryRecv(const char *buffer, size_t size, int flags) {
	return recv(this->m_fd, (void *) buffer, size, flags);
}

int MySocket::trySend(const char *buffer, size_t size, int flags) {
	return send(this->m_fd, (void *) buffer, size, flags);
}

int MySocket::closeFd(void) {
	if (!this->m_isOpen) {
		return 0;
	}
	this->m_isOpen = false;
	return close(this->m_fd);
}

bool MySocket::operator>(MySocket const &rhs) {
	return this->m_fd > rhs.m_fd;
}

bool MySocket::operator>(int fd) {
	return this->m_fd > fd;
}

bool MySocket::operator<(MySocket const &rhs) {
	return this->m_fd < rhs.m_fd;
}

bool MySocket::operator<(int fd) {
	return this->m_fd < fd;
}

bool MySocket::operator==(MySocket const &rhs) {
	return this->m_fd == rhs.m_fd;
}

bool MySocket::operator==(int fd) {
	return this->m_fd == fd;
}

bool MySocket::operator!=(MySocket const &rhs) {
	return this->m_fd != rhs.m_fd;
}

bool MySocket::operator!=(int fd) {
	return this->m_fd != fd;
}

bool MySocket::operator>=(MySocket const &rhs) {
	return this->m_fd >= rhs.m_fd;
}

bool MySocket::operator>=(int fd) {
	return this->m_fd >= fd;
}

bool MySocket::operator<=(MySocket const &rhs) {
	return this->m_fd <= rhs.m_fd;
}

bool MySocket::operator<=(int fd) {
	return this->m_fd <= fd;
}
