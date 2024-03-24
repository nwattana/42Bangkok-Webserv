#pragma once
#include <iostream>
#include <stdexcept>
#include <string>

class ConfigNotFoundException : public std::exception
{
private:
	std::string _msg;
public:
	ConfigNotFoundException(std::string msg) : _msg(msg) {};
	~ConfigNotFoundException() throw() {};
	virtual const char *what() const throw()
	{
		return _msg.c_str();
	}
};
