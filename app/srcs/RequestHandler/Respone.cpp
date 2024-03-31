#include "Response.hpp"

/*
Response  = Status-Line
			*(( general-header
			| response-header
			| entity-header ) CRLF)
			CRLF
			[ message-body ]
*/
Response::Response()
{
	std::string str;
	// Return HTTP/1.1 200 OK
	_version = "HTTP/1.1";
	_status_code = 200;
	_status_line = _version + " " + SSTR(200) + " " + getReasonPhrase(_status_code) + "\r\n";
	_header_set = "";
}

Response::Response(int status_code, std::string body)
{
	_version = "HTTP/1.1";
	_status_code = status_code;
	_status_line = _version + " " + SSTR(_status_code) + " " + getReasonPhrase(_status_code) + "\r\n";
	_header_set = "";
	_body = body;
}

Response::~Response()
{
}

Response::Response(Response const &src)
{
	*this = src;
}

Response &Response::operator=(Response const &rhs)
{
	if (this != &rhs)
	{
		// copy
	}
	return *this;
}

std::string Response::getResponse()
{
	
	_header_set += "Content-Length: " + SSTR(_body.length()) + "\r\n";
	return _status_line + _header_set + "\r\n" + _body;
}

void Response::setBody(std::string body)
{
	_body = body;
}

void Response::setStatusCode(int status_code)
{
	_status_code = status_code;
	_status_line = _version + " " + SSTR(_status_code) + " " + getReasonPhrase(_status_code) + "\r\n";
}

// check if key is already in header
// if yes, replace it
// if no, add it
void Response::setHeader(std::string key, std::string value)
{
	if (_header_set.find(key + ":") != std::string::npos)
	{
		size_t start = _header_set.find(key + ":");
		size_t end = _header_set.find("\r\n", start);
		_header_set.replace(start, end - start, key + ": " + value);
	}
	else
	{
		_header_set += key + ": " + value + "\r\n";
	}
	// std::cout << "Header set: " << _header_set << std::endl;
}