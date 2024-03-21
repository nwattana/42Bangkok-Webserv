#include "LocationRule.hpp"

LocationRule::LocationRule()
{
	_set_up_default();
}

LocationRule::LocationRule(LocationBlock block, std::string server_dir)
{
	_set_up_default();
	// std::string config_in = block.getConfig(L_METHOD_ALLOW)[0];
	// if (config_in.empty())
	// 	_allow_method = GET;
	// else
	// 	_allow_method = allow_method_parser(config_in);
}

LocationRule::LocationRule(LocationRule const &src)
{
	*this = src;
}

LocationRule &LocationRule::operator=(LocationRule const &rhs)
{
	if (this != &rhs)
	{
		// copy
	}
	return *this;
}

LocationRule::~LocationRule()
{
	std::cout << "RequestHandler::~LocationRule" << std::endl;
}

void LocationRule::_set_up_default()
{
	_full_path = "./";
	_handle_route = "./";
	_dir_path = "./";
	_root_dir = "./";
	_depth = 0;
	_allow_method = GET;
	_allow_upload = 0;
	_upload_path = "./";
	_cgi_bin = "./";
}

void LocationRule::printSetting(void) const
{
	std::cout << "LocationRule::printSetting" << std::endl;
	std::cout << "\tLocationRule: " << std::endl;
	std::cout << "\thandle_route: " << _handle_route << std::endl;
	std::cout << "\t\tfull_path: " << _full_path << std::endl;
	std::cout << "\t\tdir_path: " << _dir_path << std::endl;
	std::cout << "\t\troot_dir: " << _root_dir << std::endl;
	std::cout << "\t\tdepth: " << _depth << std::endl;
	std::cout << "\t\tallow_method: " << _allow_method << std::endl;
	std::cout << "\t\tallow_upload: " << _allow_upload << std::endl;
	std::cout << "\t\tupload_path: " << _upload_path << std::endl;
	std::cout << "\t\tcgi_bin: " << _cgi_bin << std::endl;
}