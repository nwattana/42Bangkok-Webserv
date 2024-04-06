#include "utils.hpp"

int allow_method_parser(std::string allow_method)
{
	std::vector<std::string> method_list;
	int ret = 0;

	allow_method = ft_strtrim(allow_method);
	method_list = ft_split(allow_method, ", ");
	for (size_t i = 0; i < method_list.size(); i++)
	{
		ret |= allow_method_str_to_int(method_list[i]);
	}

	return ret;
}

int allow_method_str_to_int(std::string method)
{
	for (size_t i = 0; i < method.length(); i++)
	{
		method[i] = toupper(method[i]);
	}
	if (method == "GET")
		return GET;
	else if (method == "POST")
		return POST;
	else if (method == "PUT")
		return PUT;
	else if (method == "DELETE")
		return DELETE;
	else if (method == "HEAD")
		return HEAD;
	else if (method == "OPTIONS")
		return OPTIONS;
	else if (method == "TRACE")
		return TRACE;
	else if (method == "CONNECT")
		return CONNECT;
	else
	{
		// std::cerr << RED << "ALLOW METHOD PARSER : \"" << method << "\" is not a valid method" << RESET << std::endl;
		return 0;
	}
}

std::string method_allow_to_string(int method)
{
	std::string str;
	if (method & GET)
		str += "GET ";
	if (method & POST)
		str += "POST ";
	if (method & PUT)
		str += "PUT ";
	if (method & DELETE)
		str += "DELETE ";
	if (method & HEAD)
		str += "HEAD ";
	if (method & OPTIONS)
		str += "OPTIONS ";
	if (method & TRACE)
		str += "TRACE ";
	if (method & CONNECT)
		str += "CONNECT ";
	return str;
}
