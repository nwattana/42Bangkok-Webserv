#include "../../includes/Cfile.hpp"

Cfile::Cfile() : m_fd(0), m_isOpen(false), m_isSocket(false)
{
}

Cfile::Cfile(int fd, bool isOpen, bool isSocket) : m_fd(fd), m_isOpen(isOpen), m_isSocket(isSocket)
{
}

Cfile::~Cfile()
{
}

Cfile::Cfile(Cfile const &src)
{
	*this = src;
}

Cfile &Cfile::operator=(Cfile const &rhs)
{
	if (this != &rhs)
	{
		this->m_fd = rhs.m_fd;
		this->m_isOpen = rhs.m_isOpen;
		this->m_isSocket = rhs.m_isSocket;
	}
	return *this;
}

int Cfile::toggleOpen(bool toOpen)
{
	if (toOpen)
	{
		if (!this->m_isOpen)
		{
			this->m_isOpen = true;
			return 0;
		}
	}
	else
	{
		if (this->m_isOpen)
		{
			this->m_isOpen = false;
			return close(this->m_fd);
		}
	}
	return 0;
}

int Cfile::getFd(void) const
{
	return this->m_fd;
}

bool Cfile::isOpen(void) const
{
	return this->m_isOpen;
}

bool Cfile::isSocket(void) const
{
	return this->m_isSocket;
}

int Cfile::tryRecv(const char *buffer, size_t size, int flags)
{
	if (this->m_isOpen)
		return recv(this->m_fd, (void *)buffer, size, flags);
	return -1;
}

int Cfile::trySend(const char *buffer, size_t size, int flags)
{
	if (this->m_isOpen)
		return send(this->m_fd, (void *)buffer, size, flags);
	return -1;
}

bool Cfile::operator>(Cfile const &rhs)
{
	return this->m_fd > rhs.m_fd;
}

bool Cfile::operator>(int fd)
{
	return this->m_fd > fd;
}

bool Cfile::operator<(Cfile const &rhs)
{
	return this->m_fd < rhs.m_fd;
}

bool Cfile::operator<(int fd)
{
	return this->m_fd < fd;
}

bool Cfile::operator==(Cfile const &rhs)
{
	return this->m_fd == rhs.m_fd;
}

bool Cfile::operator==(int fd)
{
	return this->m_fd == fd;
}

bool Cfile::operator!=(Cfile const &rhs)
{
	return this->m_fd != rhs.m_fd;
}

bool Cfile::operator!=(int fd)
{
	return this->m_fd != fd;
}

bool Cfile::operator>=(Cfile const &rhs)
{
	return this->m_fd >= rhs.m_fd;
}

bool Cfile::operator>=(int fd)
{
	return this->m_fd >= fd;
}

bool Cfile::operator<=(Cfile const &rhs)
{
	return this->m_fd <= rhs.m_fd;
}

bool Cfile::operator<=(int fd)
{
	return this->m_fd <= fd;
}
