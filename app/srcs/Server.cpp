#include "Server.hpp"

Server::Server(void)
{
	std::cout << "Default Constructor Can't be called" << std::endl;
}

Server::Server(std::string config)
{
	std::cout << "server constructing with config" << std::endl;
	std::cout << config << std::endl;
}

Server::~Server(void)
{
	std::cout << "Bye" << std::endl;
}

Server &Server::operator=(Server const &src)
{
	this->raw_config = src.get_config();
	return *this;
}

std::string Server::get_config(void) const
{
	return this->raw_config;
}