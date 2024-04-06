#include "utils.hpp"

std::string port_parser(std::string port)
{
	if (port.empty())
	{
		throw ParseErrorException("Port is empty");
	}
	for (size_t i = 0; i < port.length(); i++)
	{
		if (!isdigit(port[i]))
		{
			throw ParseErrorException("Port is not a number");
		}
	}
	return port;
}

std::string router_parser(std::string route)
{
	const char *disallow_char = "?#<>{}|\\^~[]`";
	if (route.empty())
	{
		throw ParseErrorException("Route is empty");
	}
	if (route[0] != '/')
	{
		throw ParseErrorException("Route is not start with /");
	}
	for (size_t i = 0; i < route.length(); i++)
	{
		if (route[i] < 32 || route[i] > 126)
		{
			throw ParseErrorException("Route contains non-printable characters");
		}
		if (strchr(disallow_char, route[i]) != NULL)
		{
			throw ParseErrorException("Route contains disallowed characters");
		}
	}
	// check consecutive slashes
	if (route.find("//") != std::string::npos)
	{
		throw ParseErrorException("Route contains consecutive slashes");
	}
	return route;
}

std::string uri_parser(std::string uri)
{
	const char *disallow_char = "?#<>{}|\\^~[]`";

	if (uri.empty())
	{
		throw ParseErrorException("URI is empty");
	}
	for (size_t i = 0; i < uri.length(); i++)
	{
		if (uri[i] < 32 || uri[i] > 126)
		{
			throw ParseErrorException("URI contains non-printable characters");
		}
		if (strchr(disallow_char, uri[i]) != NULL)
		{
			throw ParseErrorException("URI contains disallowed characters");
		}
	}

	// don't allow double dots
	if (uri.find("..") != std::string::npos)
	{
		throw ParseErrorException("URI contains double dots");
	}

	// dont allow double slashes
	if (uri.find("//") != std::string::npos)
	{
		throw ParseErrorException("URI contains double slashes");
	}

	return uri;
}

/*
 * convert Double slash in string to single slash
 */
std::string resolve_double_slash(std::string path)
{
	int pos;

	pos = path.find("//");
	while (pos != std::string::npos)
	{
		path = path.substr(0, pos) + path.substr(pos + 1);
		pos = path.find("//");
	}
	return path;
}

std::vector<std::string> http_request_split_line(std::string http_request)
{
	std::string line_seperator = "\r\n";
	std::string clone = http_request;
	std::string token;
	std::vector<std::string> res;
	int pos;

	pos = clone.find("\r\n");
	while (pos != -1)
	{
		token = clone.substr(0, pos);
		clone = clone.substr(pos + 2);
		res.push_back(token+"\r\n");
		pos = clone.find("\r\n");
	}
	return (res);
}

void test_http_request_split_line(void)
{
	std::string test_str = 
	"----------------------------033513928446945409175686\r\nContent-Disposition: form-data; name=\"\"; filename=\"small.txt.txt\"\r\nContent-Type: text/plain\r\n\r\n aaaaaaaaaa\r\n"
	"----------------------------033513928446945409175686\r\nContent-Disposition:form-data;name =\"\"; filename=\"small.txt.txt\"\r\nContent-Type: text/plain\r\n\r\naaaaaaaaaa\r\n"
	"----------------------------033513928446945409175686\r\nContent-Disposition: form-data; name=\"\"; filename=\"small.txt.txt\"\r\nContent-Type: text/plain\r\n\r\naaaaaaaaaa\r\n"
	"----------------------------033513928446945409175686\r\nContent-Disposition : form - data;name =\"abc\"\r\n\\r\n3123\r\n"
	"----------------------------033513928446945409175686--\r\n";

	std::vector<std::string > vec=http_request_split_line(test_str);
	for (std::vector<std::string>::iterator it=vec.begin(); it != vec.end(); it++)
	{
		std::cout << *it << YELLOW << "\\r"<< GREEN << "\\n" << RESET << std::endl;
	}
}


/// @brief return 0 when request line not valid
int parse_request_line(std::string request_line)
{
	std::vector<std::string> tokens = ft_split(request_line," ");
	int len = tokens.size();
	if (len != 3)
	{
		std::cerr << "invalid request line" << std::endl;
		return (0);
	}
	try{
		if (allow_method_str_to_int(tokens[0]) == 0)
		{
			std::cerr << "invalid method" << std::endl;
			return (0);
		}
		uri_parser(tokens[1]);
		if (tokens[2] != "HTTP/1.1\r\n")
		{
			std::cerr << "invalid version" << std::endl;
			return (0);
		}
	}
	catch (std::exception &e)
	{
		std::cerr << "invalue uri" << std::endl;
		return (0);
	}
	return (1);
}