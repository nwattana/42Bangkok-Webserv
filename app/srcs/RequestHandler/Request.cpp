#include "Request.hpp"

Request::Request()
{
}

Request::Request(std::string header, std::string body)
{
	_header = header;
	_body = body;

	// parse request line
	std::string request_line = _header.substr(0, _header.find("\r\n"));

	_parse_request_line(request_line);
	// parse header
	std::string header_str = _header.substr(_header.find("\r\n") + 2);
	_parse_header(header_str);

}

Request::Request(Request const &src)
{
	*this = src;
}

Request &Request::operator=(Request const &rhs)
{
	if (this != &rhs)
	{
		// copy
	}
	return *this;
}

Request::~Request()
{
}

void Request::printSetting(void) const
{
	std::cout << "==========REQUEST=============" << std::endl;
	print_request_line_info();
	std::cout << "==========HEADER==============" << std::endl;
	print_header();
	std::cout << "==========BODY================" << std::endl;
	print_body();
	std::cout << "==========/REQUEST============" << std::endl;
}

void Request::_parse_request_line(std::string line)
{
	std::string method, uri, version;
	std::stringstream ss;
	ss << line;
	ss >> method >> uri >> version;
	_request_line = line;
	_method = allow_method_str_to_int(method);
	_uri = uri;
	_version = version;

}

void Request::_parse_header(std::string header)
{
	std::string temp;
	int pos, pos2;

	pos = header.find("\r\n");
	while (pos != -1)
	{
		temp = header.substr(0, pos);
		header = header.substr(pos + 2);
		pos2 = temp.find(": ");
		if (pos2 != -1)
		{
			std::string key = temp.substr(0, pos2);
			std::string value = temp.substr(pos2 + 2);
			_header_map[key].push_back(value);
		}
		pos = header.find("\r\n");
	}
}


std::string Request::get_request_line(void)
{
	return _request_line;
}

std::string Request::get_header(std::string key)
{
	std::map<std::string, std::vector<std::string> >::iterator it;
	it = _header_map.find(key);
	if (it != _header_map.end())
	{
		return it->second[0];
	}
	throw HeaderNotFound("Request::get_header : key not found \"" + key + "\"");
}

std::string Request::get_body(void)
{
	return _body;
}

std::string Request::raw_request(void)
{
	return _header + "\r\n" + _body;
}

void Request::print_header(void) const
{
	std::map<std::string, std::vector<std::string> >::const_iterator it;
	for (it = _header_map.begin(); it != _header_map.end(); it++)
	{
		std::cout << it->first << ": ";
		for (size_t i = 0; i < it->second.size(); i++)
		{
			std::cout << it->second[i] << " ";
		}
		std::cout << std::endl;
	}
}
void Request::print_body(void) const
{
	if (_body.empty())
	{
		std::cout << "[Body Empty]" << std::endl;
		return;
	}
	std::cout << _body << std::endl;
}

void Request::print_request_line_info(void) const
{
	std::cout << "Request-Line: " << _request_line << std::endl;
	std::cout << "Method: " << _method << std::endl;
	std::cout << "URI: " << _uri << std::endl;
	std::cout << "Version: " << _version << std::endl;
}

std::string Request::get_uri(void)
{
	return _uri;
}

int Request::get_method(void)
{
	return _method;
}

void Request::set_resource(std::string resource)
{
	_resource = resource;
}

std::string Request::get_resource(void)
{
	return _resource;
}

void Request::set_body(std::string body)
{
	_body = body;
}

void Request::set_header(std::string header)
{
	_parse_header(header);
}

void Request::append_body(std::string more)
{
	_body += more;
}

// return 1 if found multipart/form-data
int Request::check_if_multipart(void)
{
	std::vector<std::string > temp;
	std::vector<std::string >::iterator it;
	try
	{
		if (_header_map.find("Content-Type") != _header_map.end())
		{
			std::vector<std::string> vec = _header_map["Content-Type"];
			temp = ft_split(vec[0], "; ");
			if (temp[0] == "multipart/form-data")
			{
				_boundary=ft_split(temp[1], "=")[1];
				return (1);
			}
		}
	}
	catch(const std::exception& e)
	{
		std::cerr << RED << e.what() << '\n';
	}
	return (0);
}

// return 1 if found boundary
int Request::check_chunk_done(void)
{
	int pos = _body.find("--" + _boundary + "--");
	if (pos == -1)
	{
		std::cout << BLUE << "Chunked not done " << RESET << std::endl;
		return (0);
	}
	std::cout << GREEN << "Chunked done " << RESET << std::endl;
	// TODO chunk corrrect position;
	return (1);
}