#include "LocationRule.hpp"

LocationRule::LocationRule()
{
	_set_up_default();
}

LocationRule::LocationRule(LocationBlock block, std::string server_dir)
{
	_set_up_default();
	std::string temp;

	_handle_route = block.getLocationMatch();
	_set_root_dir(block, server_dir);
	_set_depth();
	_set_allow_method(block);
	_set_cgi(block, server_dir);
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
	_handle_route = "/";
	_root_dir = "./";
	_depth = 0;
	_allow_method = 0;
	_allow_upload = 0;
	_upload_path = "./";
	_cgi_bin = "./";
}

bool LocationRule::operator>(LocationRule const &rhs)
{
	return (this->_depth > rhs.get_depth());
}

bool LocationRule::operator<(LocationRule const &rhs)
{
	return (this->_depth < rhs.get_depth());
}

bool LocationRule::operator==(LocationRule const &rhs)
{
	return (this->_depth == rhs.get_depth());
}

int LocationRule::get_depth(void) const
{
	return _depth;
}

void LocationRule::printSetting(void) const
{
	std::cout << "LocationRule::printSetting" << std::endl;
	std::cout << "\tLocationRule: " << std::endl;
	std::cout << "\thandle_route: " << _handle_route << std::endl;
	std::cout << "\t\tfull_path: " << _full_path << std::endl;
	std::cout << "\t\troot_dir: " << _root_dir << std::endl;
	std::cout << "\t\tdepth: " << _depth << std::endl;
	std::cout << "\t\tallow_method: " << method_allow_to_string(_allow_method) << std::endl;
	std::cout << "\t\tallow_upload: " << _allow_upload << std::endl;
	std::cout << "\t\tupload_path: " << _upload_path << std::endl;
	std::cout << "\t\tcgi_handle: " << _cgi_handle << std::endl;
	std::cout << "\t\tcgi_bin: " << _cgi_bin << std::endl;

}

bool LocationRule::match_uri_path(std::string uri) const
{
	if (strncmp(_handle_route.c_str(), uri.c_str(), _handle_route.length()) == 0)
	{
		std::cout << "Matched LocationRule : " << _handle_route << " : " << uri << std::endl;
		return true;
	}
	else
	{
		return false;
	}
}

void LocationRule::_set_handle_route(LocationBlock block)
{
	_handle_route = block.getLocationMatch();
	_set_depth();
}

void LocationRule::setRootDir(std::string root_dir)
{
	_root_dir = root_dir;
}

std::string LocationRule::getRootDir(void) const
{
	return _root_dir;
}

void LocationRule::setCGIBin(std::string cgi_bin)
{
	_cgi_bin = cgi_bin;
}

std::string LocationRule::getCGIBin(void) const
{
	return _cgi_bin;
}

void LocationRule::setUploadPath(std::string upload_path)
{
	_upload_path = upload_path;
}

std::string LocationRule::getUploadPath(void) const
{
	return _upload_path;
}

std::string LocationRule::get_handle_route(void) const
{
	return _handle_route;
}

std::string LocationRule::get_full_path(void) const
{
	return _full_path;
}

void LocationRule::setAllowMethod(int allow_method)
{
	_allow_method = allow_method;
}

int LocationRule::getAllowMethod(void) const
{
	return _allow_method;
}

void LocationRule::_set_root_dir(LocationBlock block, std::string server_dir)
{
	try{
		_root_dir = block.getConfig(L_ROOT_DIR)[0];
	}
	catch (ConfigNotFoundException)
	{
		_root_dir = server_dir;
	}
}

void LocationRule::_set_cgi_bin(LocationBlock block, std::string server_dir)
{
	std::string temp;

	temp = block.getConfig(L_CGI_HANDLER)[0];
	if (temp.empty())
		_cgi_bin = server_dir;
	else
		_cgi_bin = temp;
}

void LocationRule::_set_allow_method(LocationBlock block)
{
	std::vector<std::string> temp = block.getConfig(L_METHOD_ALLOW);

	for (size_t i = 0; i < temp.size(); i++)
	{
		_allow_method |= allow_method_parser(temp[i]);
	}
}


void LocationRule::_set_upload(LocationBlock block)
{
	std::string temp;
	try
	{
		temp = block.getConfig(L_UPLOAD_PATH)[0];
	}
	catch (ConfigNotFoundException)
	{
		_allow_upload = 0;
		return;
	}
	try
	{
		temp = block.getConfig(L_UPLOAD_PATH)[0];
	}
	catch(const std::exception& e)
	{
		_upload_path = _root_dir;
	}
}


void LocationRule::_set_depth(void)
{
	if (_handle_route == "/")
		_depth = 0;
	else
		_depth = ft_split(_handle_route, "/").size();
}

void LocationRule::_set_cgi(LocationBlock block, std::string server_dir)
{
	std::string temp_str;
	try
	{
		temp_str = block.getConfig(L_CGI_HANDLER)[0];
		if (temp_str != "enable")
		{
			_cgi_handle = 0;
			return ;
		}
		_cgi_handle = 1;
	}
	catch (ConfigNotFoundException)
	{
		_cgi_handle = 0;
		return ;
	}
	try
	{
		_cgi_bin = block.getConfig(L_CGI_PATH)[0];
	}
	catch(const std::exception& e)
	{
		_cgi_bin = server_dir;
	}
}