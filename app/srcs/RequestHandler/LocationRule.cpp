#include "LocationRule.hpp"

LocationRule::LocationRule()
{
	_set_up_default();
}

LocationRule::LocationRule(LocationBlock &block, std::string server_dir, std::string server_name, std::string server_port)
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
	set_auto_index(block);
	_set_cwd();
	_set_upload(block);
	this->_host_name = server_name;
	this->_server_port = server_port;
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
	// delete _m_response_handler;
}

void LocationRule::_set_up_default()
{
	_handle_route = "/";
	_root_dir = "./";
	_depth = 0;
	_allow_method = 0;
	_allow_upload = 0;
	_upload_path = "./";
	_cgi_script = "./";
	_cgi_handle = 0;
	_set_cwd();
	_auto_index_cgi = "./cgi_bin/py/auto_index.py";
	_is_set_index_page = 0;
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
	std::cout << "\t\troot_dir: " << _root_dir << std::endl;
	std::cout << "\t\tdepth: " << _depth << std::endl;
	std::cout << "\t\tallow_method: " << method_allow_to_string(_allow_method) << std::endl;
	std::cout << "\t\tallow_upload: " << _allow_upload << std::endl;
	std::cout << "\t\tupload_path: " << _upload_path << std::endl;
	std::cout << "\t\tcgi_handle: " << _cgi_handle << std::endl;
	std::cout << "\t\tcgi_bin: " << _cgi_script << std::endl;
}

bool LocationRule::match_uri_path(std::string uri) const
{
	if (strncmp(_handle_route.c_str(), uri.c_str(), _handle_route.length()) == 0)
	{
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
	_cgi_script = cgi_bin;
}

std::string LocationRule::getCGIBin(void) const
{
	return _cgi_script;
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
	catch (ConfigNotFoundException &e)
	{
		_root_dir = server_dir;
	}
}

void LocationRule::_set_cgi_bin(LocationBlock block, std::string server_dir)
{
	std::string temp;

	temp = block.getConfig(L_CGI_HANDLER)[0];
	if (temp.empty())
		_cgi_script = server_dir;
	else
		_cgi_script = temp;
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
		_allow_upload = 1;
	}
	catch (ConfigNotFoundException &e)
	{
		_allow_upload = 0;
		return;
	}
	try
	{
		temp = block.getConfig(L_UPLOAD_PATH)[0];
		_upload_path = temp;
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
	catch (ConfigNotFoundException &e)
	{
		_cgi_handle = 0;
		return;
	}

	try
	{
		_cgi_script = _cwd + "/" + block.getConfig(L_CGI_SCRIPT)[0];
		is_set_cgi_script = 1;
	}
	catch (ConfigNotFoundException &e)
	{
		_cgi_script = server_dir;
		is_set_cgi_script = 0;
	}
}

void LocationRule::_set_index_page(LocationBlock block)
{
	std::string temp;
	try
	{
		temp = block.getConfig(L_INDEX)[0];
	}
	catch (ConfigNotFoundException &e)
	{
		_index_page = "index.html";
		_is_set_index_page = 0;
		return;
	}
	_index_page = temp;
	_is_set_index_page = 1;
}

// join root and index page if index page at location not set use default index page
std::string LocationRule::get_index_page(void) const
{
	return _index_page;
}

void LocationRule::setIndexPage(std::string index_page, int defalt)
{
	_index_page = index_page;
	_is_set_index_page = defalt;
}

std::string LocationRule::set_auto_index(std::string auto_index)
{
	if (auto_index == "enable")
		_auto_index = auto_index;
	else if (auto_index == "disable")
		_auto_index = auto_index;
	else
		throw ConfigNotFoundException("in valid auto index value");
	return _auto_index;
}

std::string LocationRule::set_auto_index(LocationBlock &block)
{
	std::string temp;
	try
	{
		temp = block.getConfig(L_AUTOINDEX)[0];
		temp = ft_strtrim_char_set(temp, ";");
	}
	catch (ConfigNotFoundException &e)
	{
		_auto_index = "disable";
		return _auto_index;
	}
	if (temp == "enable")
		_auto_index = temp;
	else if (temp == "disable")
		_auto_index = temp;
	else
	{
		throw ConfigNotFoundException("in valid auto index value");
	}
	return _auto_index;
}

/// @brief generate resource path from uri path
/// @param uri
/// @return actual path to resource in current server
std::string LocationRule::generate_resource_GET(std::string uri)
{
	std::string result, path_till_handle_route, remaining_path;

	path_till_handle_route = uri.substr(_handle_route.length());
	remaining_path = uri.substr(path_till_handle_route.length(), uri.length());
	result = _cwd + "/" + _root_dir + "/" + path_till_handle_route;

	if (result[result.length() - 1] == '/')
	{
		if (_auto_index == "enable" && _is_set_index_page == 0)
		{
			// add space for auto index after this will split later
			result = _auto_index_cgi + " " + result;
		}
		else
		{
			_add_index_page(result);
		}
	}
	result = resolve_double_slash(result);
	return (result);
}

std::string LocationRule::generate_resource_POST(std::string uri)
{
	std::string result, path_till_handle_route, remaining_path;

	path_till_handle_route = uri.substr(_handle_route.length());
	remaining_path = uri.substr(path_till_handle_route.length(), uri.length());

	result = _cwd + "/" + _root_dir + "/" + path_till_handle_route;
	result = resolve_double_slash(result);
	return (result);
}

std::string LocationRule::generate_resource_DELETE(std::string uri)
{
	std::string result, path_till_handle_route, remaining_path;

	// check กอน ตัด
	if (uri[uri.length() - 1] == '/')
	{
		return "error:f57e9322ebe34807a0de7cf52dc52288";
	}
	path_till_handle_route = uri.substr(_handle_route.length());
	result = _cwd + "/" + _root_dir + "/" + path_till_handle_route;
	result = resolve_double_slash(result);
	std::cout <<RED << "path to delete " << result << RESET << std::endl;
	return (result);
}

/// @brief Check if uri is a directory or a file
/// @param uri
void LocationRule::_add_index_page(std::string &uri)
{
	if (uri.empty() || uri == "/" || uri.end()[-1] == '/')
	{
		uri += _index_page;
	}
}

void LocationRule::set_error_page(std::map<int, std::string> error_page)
{
	_error_page = error_page;
}

void LocationRule::_set_error_page(std::map<int, std::string> error_page)
{
	std::map<int, std::string>::const_iterator it;
	_error_page = error_page;

	if (LOCATION_RULE_DEBUG)
	{
		for (it = _error_page.begin(); it != _error_page.end(); it++)
		{
			std::cout << "Error Page : " << it->first << " : " << it->second << std::endl;
		}
	}
}

void LocationRule::location_print_error_page(void) const
{
	std::map<int, std::string>::const_iterator it;

	db_red("NO error page found");
	if (_error_page.empty())
	{
		return;
	}
	for (it = _error_page.begin(); it != _error_page.end(); it++)
	{
		if (LOCATION_RULE_DEBUG)
		{
			std::cout << "LocationRule::location_print_error_page Error Page : " << it->first << " : " << it->second << std::endl;
		}
	}
}

/// @brief Create error response depending status code
/// @param status_code
/// @return
std::string LocationRule::location_generate_simple_response(int status_code)
{
	std::string res;
	std::string path;

	try
	{

		std::cout << "Status Code : " << status_code << std::endl;
		path = _error_page.at(status_code);
		db_red(path);
	}
	catch (const std::exception &e)
	{
		// std::cerr << e.what() << '\n';
		this->location_print_error_page();
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

std::string LocationRule::_handle_get(Request &request)
{
	std::string res;
	std::string resource = generate_resource_GET(request.get_uri());

	// try to read file
	if (resource[resource.length() - 1] == '/' && _auto_index == "enable")
	{
		db_blue("GET Resource [auto index] [path] : " + std::string(YELLOW) + resource);
		ExecveAdapter adaptor(resource, std::vector<std::string>());
		res = execute_cgi(2, adaptor.get_argv(), adaptor.get_envp());
		if (res == "2980f025-918e-42dc-a0bc-843452f04847-timeout")
			return location_generate_simple_response(408);
		Response response(200, res);
		response.setHeader("Content-Type", "text/html");
		adaptor.destory();
		return response.getResponse();
	}
	else
	{
		db_blue("GET Resource [path] : " + std::string(YELLOW) + resource);
		try
		{
			StaticFileWaiter static_file_waiter(resource);
			std::string body = static_file_waiter.read_file();
			Response response(200, body);
			response.setHeader("Content-Type", static_file_waiter.get_content_type());
			res = response.getResponse();
		}
		catch (FileNotAccessibleException &e)
		{
			res = location_generate_simple_response(404);
		}
	}
	// std::cout << res << std::endl;
	return res;
}

std::string LocationRule::_handle_post(Request &request)
{
	std::string res;
	std::string resource = generate_resource_POST(request.get_uri());

	try{
		std::string filename = request.get_header("Content-Disposition");
		filename = filename.substr(filename.find("filename=") + 10);
		filename = filename.substr(0, filename.length() - 1);
		if (resource[resource.length() - 1] != '/')
		{
			return location_generate_simple_response(409);
		}
		resource += filename;
	}
	catch(const std::exception &e){
	}
	FileCreator file_crator(resource);
	if (file_crator.check_file_exist(file_crator.getFileName()) == 1)
	{
		res = location_generate_simple_response(409);
	}
	else
	{
		file_crator.setContent(request.get_body());
		file_crator.createFile(CREATE_FILE);
		try
		{
			StaticFileWaiter static_file_waiter(resource);
			std::string body = static_file_waiter.read_file();
			Response response(201, body);
			response.setHeader("Content-Type", static_file_waiter.get_content_type());
			res = response.getResponse();
		}
		catch (FileNotAccessibleException &e)
		{
			res = location_generate_simple_response(500);
		}
	}
	return res;
}

std::string LocationRule::execute_request(Request &request)
{
	int method = request.get_method();
	db_blue("call uri " + std::string(YELLOW)  + request.get_uri());

	// Method Not allow
	if (!(method && _allow_method))
	{
		db_blue("not allow" + _handle_route + method_allow_to_string(method));
		return location_generate_simple_response(405);
	}

	// check route function
	// directory list
	// read static file
	// upload
	// download

	if (_cgi_handle)
	{
		return cgi_call(request);
	}
	else
	{
		return _build_in_cgi(request);
	}
	return location_generate_simple_response(200);
}

std::string LocationRule::_build_in_cgi(Request &request)
{
	int method = request.get_method();
	int status = 501;
	int create_chunk_data = request.create_chunk_data();


	db_blue("Build In CGI CALL method : " + std::string(YELLOW) + method_allow_to_string(request.get_method()));
	if (create_chunk_data && method & POST && _allow_upload)
	{
		std::cout << RED << "UPLOAD" << RESET << std::endl;
		status = _handle_upload(request);
		return location_generate_simple_response(status);
	}
	if (method & DELETE && method & _allow_method)
	{
		std::cout << "DELETE FILE " << RESET << std::endl;
		status = _handle_delete_file(request);
		return location_generate_simple_response(status);
	}

	if (method & GET)
	{
		return _handle_get(request);
	}
	else
	{
		return location_generate_simple_response(405);
	}
}

std::string LocationRule::cgi_call(Request &request)
{
	// CRUD
	std::cout << GREEN;
	std::string requested_file = cgi_parse_url(request);
	std::string raw_request = request.raw_request();

	if (_cgi_script.empty())
	{
		std::cerr << "CGI Script not set" << std::endl;
		return location_generate_simple_response(500);
	}

	std::cout << "CGI Script : " << _cgi_script << std::endl;
	std::cout << "PATH TO FILE  : " << _cwd + "/" + requested_file << std::endl;
	ExecveAdapter adaptor(_cgi_script, std::vector<std::string>());
	adaptor.add_env("RAW_REQUEST", raw_request);
	adaptor.add_env("REQUEST_METHOD", "GET");
	adaptor.add_env("SERVER_PROTOCOL", "HTTP/1.1");
	adaptor.add_env("HOST_NAME", _host_name);
	adaptor.add_env("CGI_ROUTE", _handle_route);
	adaptor.add_env("SERVER_PORT", _server_port);
	adaptor.add_env("PATH_TO_FILE", _cwd + "/" + requested_file);
	adaptor.add_env("ROOT_DIR", _root_dir);
	adaptor.add_env("AUTO_INDEX", _auto_index);
	adaptor.add_env("UPLOAD_PATH", _upload_path);
	adaptor.add_env("INDEX_PAGE", _index_page);
	adaptor.add_env("ALLOW UPLOAD", SSTR(_allow_upload));
	adaptor.add_env("ALLOW_METHOD", method_allow_to_string(_allow_method));

	char **envp = adaptor.get_envp();

	// DEBUG PURPOSE
	// char **temp;
	// temp = envp;
	// print 2 d array
	// std::cout << BLUE;
	// while (*temp)
	// {
	// 	std::cout << *temp << std::endl;
	// 	temp++;
	// }
	// std::cout << RESET;


	std::string res = execute_cgi(2, adaptor.get_argv(), envp);
	if (res == "2980f025-918e-42dc-a0bc-843452f04847-timeout")
		return location_generate_simple_response(504);

	std::cout << YELLOW << res << std::endl;
	adaptor.destory();

	return (res);
}

// Path: in webserver machine
std::string LocationRule::cgi_parse_url(Request& request)
{
	std::string uri = request.get_uri();
	std::string res = "";
	std::string front_path = uri.substr(0, uri.find(_handle_route));
	std::string back_path = uri.substr(uri.find(_handle_route) + _handle_route.length());

	res = front_path + _root_dir + back_path;
	std::cout << "Path resolve " << res << RESET <<std::endl;
	return (res);
}

void LocationRule::set_host_name(std::string host_name)
{
	_host_name = host_name;
}

void LocationRule::set_server_port(std::string server_port)
{
	_server_port = server_port;
}

int LocationRule::setAllowMethod(std::vector<std::string> allow_method)
{
	for (size_t i = 0; i < allow_method.size(); i++)
	{
		_allow_method |= allow_method_parser(allow_method[i]);
	}
	return (_allow_method);
}

void LocationRule::set_root_dir(std::string root_dir)
{
	_root_dir = root_dir;
}

void LocationRule::set_method_allow(std::vector<std::string> allow_method)
{
	for (size_t i = 0; i < allow_method.size(); i++)
	{
		_allow_method |= allow_method_parser(allow_method[i]);
	}
}

int LocationRule::_handle_upload(Request &request)
{
	std::vector<ChunkData> temp = request.get_chunk_data();
	FileCreator f;
	size_t i = 0, max_size=temp.size();
	int res;

	while (i < max_size)
	{
		temp[i].update_content();
		if (f.allow_type(temp[i].content_type) != 201)
		{
			db_red("Not allow Content type to upload");
			return (415);
		}

		if (temp[i].name == "")
			f.setFileName(temp[i].file_name);
		else
			f.setFileName(temp[i].name);
		f.setContent(temp[i].data);
		f.append_path(_upload_path);
		res = f.createFile(CREATE_FILE);
		f.clean();
		i++;
	}
	return (res);
}

int LocationRule::_handle_delete_file(Request &request)
{
	std::string path = generate_resource_DELETE(request.get_uri());
	FileCreator f;

	std::cout << "Path = " << path << std::endl;
	if (path == ERROR_RESOURCE)
	{
		db_red("DELETE FILE ERROR FILE PATH : " + path);
		return (400);
	}
	if (f.check_file_exist(path))
	{
		std::cout << BLUE << "DELETE FILE " << YELLOW << path << RESET << std::endl;
		if (remove(path.c_str()) == -1)
		{
			return (404);
		}
		return (204);
	}
	return (404);
}
