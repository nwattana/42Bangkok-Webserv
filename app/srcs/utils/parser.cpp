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