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
	_set_index_page(block);
	_set_error_page(block.getErrorPage());
	std::cout << "Index Page : " << _index_page << std::endl;
	_set_cwd();
	std::cout << "CWD : " << _cwd << std::endl;
	_m_response_handler = new ResponseHandler();
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
	delete _m_response_handler;
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
	_cgi_handle = 0;
	_set_cwd();
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
	try
	{
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
	catch (const std::exception &e)
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
			return;
		}
		_cgi_handle = 1;
	}
	catch (ConfigNotFoundException)
	{
		_cgi_handle = 0;
		return;
	}
	try
	{
		_cgi_bin = block.getConfig(L_CGI_PATH)[0];
	}
	catch (const std::exception &e)
	{
		_cgi_bin = server_dir;
	}
}

void LocationRule::_set_index_page(LocationBlock block)
{
	std::string temp;
	try
	{
		temp = block.getConfig(L_INDEX)[0];
	}
	catch (ConfigNotFoundException)
	{
		_index_page = "index.html";
		return;
	}
	_index_page = temp;
}

// join root and index page if index page at location not set use default index page
std::string LocationRule::get_index_page(void) const
{
	return _index_page;
}

// TODO work on this
std::string LocationRule::generate_response(Request * request)
{
	std::string res;

	if (request->get_method() & _allow_method)
	{
		std::string resource = request->get_resource();
		std::cout << "LocationRule::generate_response : resouce : | " << resource << " | " << std::endl;
		if (_cgi_handle)
		{
			ExecveAdapter execve_adapter(resource, std::vector<std::string>());
			res = execute_cgi(execve_adapter.get_argv_size(),execve_adapter.get_argv(), execve_adapter.get_envp());
			execve_adapter.destory();
		}
		else
		{
			try
			{
				StaticFileWaiter static_file_waiter(resource);
				std::string body = static_file_waiter.read_file();
				Response response(200, body);
				response.setHeader("Content-Type", static_file_waiter.get_content_type());
				return response.getResponse();
			}
			catch (FileNotAccessibleException)
			{
				res = generate_error_response(404);
			}
		}
	}
	else
	{
		res = generate_error_response(405);
	}
	if (res.empty())
	{
		res = generate_error_response(404);
	}
	return res;
}

void LocationRule::setIndexPage(std::string index_page)
{
	_index_page = index_page;
}

/// @brief generate resource path from uri path
/// @param uri 
/// @return actual path to resource in current server
std::string LocationRule::generate_request_resource(std::string uri)
{
	std::string result, path_till_handle_route, remaining_path;
	path_till_handle_route = uri.substr(_handle_route.length());
	remaining_path = uri.substr(path_till_handle_route.length(), uri.length());
	// NOTE Hard Code "/" for now handdle path like ./www/
	result = _cwd + "/" + _root_dir + "/" + _handle_route + "/" + path_till_handle_route;

	if (result.empty() || result == "/" || result.end()[-1] == '/')
	{
		std::cout << "LocationRule::generate_request_resource : resource is empty or end with /" << std::endl;
		result += _index_page;
	}
	result = resolve_double_slash(result);
	return (result);
}

void LocationRule::set_error_page(std::map<int, std::string> error_page)
{
	_error_page = error_page;
}


void LocationRule::_set_error_page(std::map<int, std::string> error_page)
{
	_error_page = error_page;
}


/// @brief Create error response depending status code
/// @param status_code 
/// @return 
std::string LocationRule::generate_error_response(int status_code)
{
	std::string res;
	std::string path;

	try
	{
		path = _error_page.at(status_code);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	
	if (path.empty())
	{
		// use default error
		Response response(status_code, "");
		res = response.getResponse();
	}
	else
	{
		// serve a static file
		Response response(status_code, read_file(path));
		response.setHeader("Content-Type", "text/html");
		res = response.getResponse();
	}
	return (res);
}

void LocationRule::_set_cwd(void)
{
	char cwd[PATH_MAX];
	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("getcwd() error");
	}
	_cwd = std::string(cwd);
}
