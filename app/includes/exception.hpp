#pragma once
#include <iostream>
#include <stdexcept>
#include <string>

class CustomExceptionBase : public std::exception
{
protected:
	std::string _msg;

public:
	CustomExceptionBase(std::string msg) : _msg(msg){};
	~CustomExceptionBase() throw(){};
	virtual const char *what() const throw()
	{
		return _msg.c_str();
	}
};

class ConfigNotFoundException : public CustomExceptionBase
{
public:
	ConfigNotFoundException(std::string msg) : CustomExceptionBase(msg){};
};

class FileNotAccessibleException : public CustomExceptionBase
{
public:
	FileNotAccessibleException(std::string msg) : CustomExceptionBase(msg){};
};

class DelimiterNotFoundException : public CustomExceptionBase
{
	public:
		DelimiterNotFoundException(std::string msg) : CustomExceptionBase(msg){};
};

class ParseErrorException : public CustomExceptionBase
{
public:
	ParseErrorException(std::string msg) : CustomExceptionBase(msg){};
};

class EmptyStringException : public CustomExceptionBase
{
public:
	EmptyStringException(std::string msg) : CustomExceptionBase(msg){};
};