#include "Request.hpp"

Request::Request()
{
	_boundary = DEFAULT_BOUNDARY;
	_header = "";
	_body = "";
}

Request::Request(std::string header, std::string body)
{
	_header = header;
	_body = body;
	_boundary = DEFAULT_BOUNDARY;

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
	std::string end=std::string("--" + _boundary + "--" + "\r\n");
	int b_len = end.length();
	int spos = _body.length() - b_len;
	int res, i = 0;
	while (b_len > i)
	{
		if (_body[spos + i] != end[i])
		{
			std::cout << _body[spos + i] << " != " << end[i] << std::endl;
			res = 0;
			break;
		}
		i++;
		res = 1;
	}
	if (res == 0)
	{
		return (0);
	}
	return (1);
}


// return 1 success 0 not multipart request -1 something error
int Request::create_chunk_data(void)
{
	if (_boundary == DEFAULT_BOUNDARY)
	{
		return (0);
	}
	std::vector<std::string> temp_con = http_request_split_line(_body), temp_merge;
	std::vector<std::string>::iterator str_it;
	std::string sep, end;
	ChunkData *new_c= NULL;
	int new_data = 0, is_end = 0;
	sep = "--" + _boundary + "\r\n";
	end = "--" + _boundary + "--\r\n";
	for (str_it=temp_con.begin(); str_it != temp_con.end(); str_it++)
	{
		if (is_end == 1)
		{
			// clean data
			return (-1);
		}
		if ((*str_it).compare(sep) == 0)
		{
			if (new_c != NULL)
			{
				_chunk_data.push_back(*new_c);
				delete new_c;
				new_c = NULL;
			}
			new_c = new ChunkData();
			new_data = 1;
		}
		else if ((*str_it).compare("\r\n") == 0)
		{
			new_data = 0;
		}
		else if ((*str_it).compare(end) == 0)
		{
			if(new_c != NULL)
			{
				_chunk_data.push_back(*new_c);
				delete new_c;
				new_c = NULL;
				is_end = 1;
			}
		}
		else if (new_data)
		{
			if (new_c->add_header(*str_it))
			{
			}
			// new_c->update_content();
		}
		else if (new_data == 0)
		{
			new_c->append_data(*str_it);
		}
		else
		{
			std::cerr << RED << "Unexpected token " << *str_it << RESET << std::endl;
		}
	}
	if (is_end)
	{
		return (1);
	}
	return (0);
}

std::vector<ChunkData> Request::get_chunk_data(void)
{
	return this->_chunk_data;
}