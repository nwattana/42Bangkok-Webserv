#include "../../includes/CfileManager.hpp"

CfileManager::CfileManager()
{
}

CfileManager::~CfileManager()
{
	this->closeAll();
}

CfileManager::CfileManager(CfileManager const &src)
{
	*this = src;
}

CfileManager &CfileManager::operator=(CfileManager const &rhs)
{
	if (this != &rhs)
	{
		this->m_cfileList = rhs.m_cfileList;
	}
	return *this;
}

Cfile CfileManager::get(int fd)
{
	return this->m_cfileList[fd];
}

void CfileManager::add(int fd, bool isOpen, bool isSocket)
{
	Cfile newCfile(fd, isOpen, isSocket);
	this->m_cfileList[fd] = newCfile;
}

void CfileManager::remove(int fd)
{
	this->m_cfileList[fd].toggleOpen(false);
	this->m_cfileList.erase(fd);
}

int CfileManager::toggleOpen(int fd, bool toOpen)
{
	return this->m_cfileList[fd].toggleOpen(toOpen);
}

int CfileManager::closeAll(void)
{
	int ret = 0;
	std::map<int, Cfile>::iterator it = this->m_cfileList.begin();
	while (it != this->m_cfileList.end())
	{
		ret = it->second.toggleOpen(false);
		if (ret < 0)
			return ret;
		it++;
	}
	return ret;
}

int CfileManager::tryRecv(int fd, const char *buffer, size_t size, int flags)
{
	return this->m_cfileList[fd].tryRecv(buffer, size, flags);
}

int CfileManager::trySend(int fd, const char *buffer, size_t size, int flags)
{
	return this->m_cfileList[fd].trySend(buffer, size, flags);
}

int CfileManager::getMaxFd(void)
{
	int max = 0;
	std::map<int, Cfile>::iterator it = this->m_cfileList.begin();
	while (it != this->m_cfileList.end())
	{
		if (it->first > max)
			max = it->first;
		it++;
	}
	return max;
}
