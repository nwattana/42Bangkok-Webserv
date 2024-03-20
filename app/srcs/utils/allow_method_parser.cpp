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
		return 0;
}