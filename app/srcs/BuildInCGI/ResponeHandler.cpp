#include "ResponseHandler.hpp"
/*
Response  = Status-Line
			*(( general-header
			| response-header
			| entity-header ) CRLF)
			CRLF
			[ message-body ]
*/
ResponseHandler::ResponseHandler()
{
	std::string str;
}

ResponseHandler::~ResponseHandler()
{
}

ResponseHandler::ResponseHandler(ResponseHandler const &src)
{
	*this = src;
}

ResponseHandler &ResponseHandler::operator=(ResponseHandler const &rhs)
{
	if (this != &rhs)
	{
		// copy
	}
	return *this;
}


void ResponseHandler::add_error_page(std::string path, int status_code)
{
	_error_page[status_code] = path;
}

std::string ResponseHandler::generate_error_response(int status_code)
{
	// Response response;
	std::string path = _error_page.at(status_code);
	if (path.empty())
	{
		// use default error page
	}
	std::string res = read_file(path);

	// return (response);
	return (res);
}