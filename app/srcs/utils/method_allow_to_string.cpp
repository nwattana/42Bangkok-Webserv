#include "utils.hpp"

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
