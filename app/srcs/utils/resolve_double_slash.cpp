#include "utils.hpp"

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
