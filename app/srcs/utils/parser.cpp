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