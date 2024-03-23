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
	std::string res = _root_dir + _index_page;
	res = resolve_double_slash(res);
	return res;
}


std::string LocationRule::generate_response(Request * request)
{
	std::string res;

	if (request->get_method() & _allow_method)
	{
		if (_cgi_handle)
		{
			// cgi
			// execute cgi and return response
			std::cout << "Not implemented yet" << std::endl;
			res = "HTTP/1.1 501 Not Implemented\r\nContent-Type: text/plain\r\nContent-Length: 0\r\n\r\n";
		}
		else
		{
			// serve a static file
			std::string resource = request->get_resource();
			// TODO ไป set index Page ตอน match path ต้อง เอา ส่วนที่ไม่เหมือนมา ต่อ root dir ด้วย
			std::cout << "LocationRule::generate_response : resouce : | " << resource << " | " << std::endl;
			std::string body = read_file(resource);
			Response response(200, body);
			return response.getResponse();
		}
	}
	else
	{
		// method not allowed return error
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


void LocationRule::set_request_resource(Request * request)
{
	std::string uri = request->get_uri();

	// trim handle route
	uri = uri.substr(_handle_route.length());
	uri = _root_dir + uri;
	std::string resource = uri.substr(_handle_route.length());
	if (resource.empty() || resource == "/" || resource.end()[-1] == '/')
		resource = get_index_page();
	request->set_resource(uri);
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