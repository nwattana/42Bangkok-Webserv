#include "RequestHandler.hpp"

RequestHandler::RequestHandler()
{
	_allowed_method = GET | POST;
	number_called = 0;
}

RequestHandler::RequestHandler(ServerConfig serverConfig)
{
	// set allowed method
	number_called = 0;
	std::string config_in = serverConfig.getConfig(S_METHOD_ALLOW)[0];
	if (config_in.empty())
		_allowed_method = GET | POST;
	else
		_allowed_method = allow_method_parser(config_in);

	// set root dir
	_root_dir = serverConfig.getConfig(S_ROOT)[0];

	// Location Rule to Match Location when recieved request

	// Default Rule for unmatched location
	create_defualt_rule();
	this->_default_rule->set_error_page(serverConfig.getErrorPage());
	try
	{
		this->_default_rule->setIndexPage(serverConfig.getConfig(S_INDEX)[0], 1);
	}
	catch (ConfigNotFoundException &e)
	{
		std::cerr << "RequestHandler::RequestHandler : " << e.what() << std::endl;
		this->_default_rule->setIndexPage("index.html", 0);
	}

	// Other location rules
	std::vector<LocationBlock>& location_configs = serverConfig.getLocationConfig();
	create_location_rule(location_configs);


	// set auto index for default rule
	try{
		std::string auto_index = serverConfig.getConfig(S_AUTO_INDEX)[0];
		if (auto_index == "enable")
			_default_rule->set_auto_index("enable");
	}
	catch (ConfigNotFoundException &e)
	{
		std::cerr << "RequestHandler::RequestHandler : " << e.what() << std::endl;
	}
}

RequestHandler::RequestHandler(RequestHandler const &src)
{
	*this = src;
}

RequestHandler &RequestHandler::operator=(RequestHandler const &rhs)
{
	if (this != &rhs)
	{
		// copy
		std::cout << "Copy Constructor of RequestHandler call but it no implement yet." << std::endl;
	}
	return *this;
}

RequestHandler::~RequestHandler()
{
	db_yellow("Warning!! RequestHandler::~RequestHandler");
	for (std::vector<LocationRule *>::iterator it = _locations_rules.begin(); it != _locations_rules.end(); it++)
	{
		delete *it;
	}
	delete _default_rule;
}

int RequestHandler::check_allowed_method(int method)
{
	return (method & _allowed_method);
}

// accept request and return response string
std::string RequestHandler::read_request(std::string request)
{
	std::stringstream ss;
	std::string methode, uri, version;
	std::cout << BLUE << "RequestHandler::read_request : " << number_called << RESET << std::endl;
	number_called++;
	// seperate header and body
	int seperator = request.find("\r\n\r\n");
	if (seperator == -1)
	{
		return _default_rule->generate_error_response(400);
	}
	// Create Request Object
	std::string header, body;
	header = request.substr(0, seperator + 2);
	body = request.substr(seperator + 4);
	_current_request = new Request(header, body);

	try
	{
		std::string req_content_type = _current_request->get_header("Content-Type");
		std::cout << RED << "header : " << header << RESET << std::endl;
		if (req_content_type.find("multipart/form-data") != std::string::npos)
		{
			_current_request->print_header();
			std::string boundary = req_content_type.substr(req_content_type.find("boundary=") + 9);
			std::cout << boundary << std::endl;
			std::string chunked_body =  add_or_append_chunked(boundary, _current_request->get_body());

			if (get_chunked_state(boundary) == 2)
			{
				std::string chunked_merge = pop_chunked(boundary);
				seperator = chunked_merge.find("\r\n\r\n");
				header = chunked_merge.substr(boundary.length(), chunked_merge.find("\r\n\r\n") + 4 - boundary.length());
				body = chunked_merge.substr(seperator + 4);
				body = body.substr(0, body.find(boundary) - 4);
				_current_request->set_body(body);
				_current_request->set_header(header);
			}
			else
			{
				return generate_error_response("/", 206);
			}

		}
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	

	uri = _current_request->get_uri();
	LocationRule *rule = _find_location_rule(uri);
	if (rule == NULL)
	{
		rule = _default_rule;
	}
	std::string response = rule->execute_request(*_current_request);
	// Clean up
	delete _current_request;

	return response;
}

void RequestHandler::create_location_rule(std::vector<LocationBlock>& location_config)
{
	LocationRule *rule;
	// for each location block, create a location rule
	for (std::vector<LocationBlock>::iterator it = location_config.begin(); it != location_config.end(); it++)
	{
		rule = new LocationRule(*it, _root_dir);
		if (rule == NULL)
		{
			std::cerr << "RequestHandler::create_location_rule : failed to create location rule" << std::endl;
			continue;
		}
		_locations_rules.push_back(rule);
	}
}

void RequestHandler::printSetting(void) const
{
	std::cout << "Allowed Method : " << method_allow_to_string(_allowed_method) << std::endl;
	std::cout << "Root Dir : " << _root_dir << std::endl;
	for (std::vector<LocationRule *>::const_iterator it = _locations_rules.begin(); it != _locations_rules.end(); it++)
	{
		(*it)->printSetting();
	}
}

LocationRule *RequestHandler::_find_location_rule(std::string uri)
{
	LocationRule *rule, *res;
	// for each location rule, check if uri match and most depth

	// Search for last '/' in uri check it followed by a file or not
	uri = uri.find_last_of("/") == uri.size() - 1 ? uri : uri.substr(0, uri.find_last_of("/") + 1);
	res = NULL;
	for (std::vector<LocationRule *>::iterator it = _locations_rules.begin(); it != _locations_rules.end(); it++)
	{
		rule = *it;
		if (rule->match_uri_path(uri))
		{
			if (res == NULL || *res < *rule)
				res = rule;
		}
	}
	return res;
}

// TODO alot of bug here
void RequestHandler::create_defualt_rule(void)
{
	_default_rule = new LocationRule();
	_default_rule->setRootDir(_root_dir);
	_default_rule->setAllowMethod(_allowed_method);
}

/// @brief Serve a error response asoociated with the error code and route
/// @return
std::string RequestHandler::generate_error_response(std::string route, int error_code)
{
	LocationRule *rule = _find_location_rule(route);
	if (rule == NULL)
	{
		rule = _default_rule;
	}
	return rule->generate_error_response(error_code);
}


std::string RequestHandler::add_or_append_chunked(std::string _key, std::string _body)
{
	std::map<std::string, std::string>::iterator it;
	std::map<std::string, int>::iterator it_state;
	it = _chunk_request.find(_key);
	it_state = _chunked_state.find(_key);

	if (it != _chunk_request.end())
	{
		it->second.append(_body);
	}
	else
	{
		_chunk_request.insert(std::pair<std::string, std::string>(_key, _body));
	}

	if (it_state == _chunked_state.end())
	{
		_chunked_state.insert(std::pair<std::string, int>(_key, 0));
	}

	it_state = _chunked_state.find(_key);
	std::string target = _key;
	size_t pos = 0;
	while ((pos = _body.find(target, pos)) != std::string::npos)
	{
		_chunked_state[_key] += 1;
		pos += target.length();
	}

	return _chunk_request[_key];
}


short RequestHandler::get_chunked_state(std::string _key)
{
	std::map<std::string, int>::iterator it_state;
	it_state = _chunked_state.find(_key);
	if (it_state != _chunked_state.end())
	{
		return it_state->second;
	}
	return 0;
}

short RequestHandler::debug_chunked_state(void)
{
	std::map<std::string, std::string>::iterator it;
	std::map<std::string, int>::iterator it_state;

	for (it = _chunk_request.begin(); it != _chunk_request.end(); it++)
	{
		std::cout << SEP << "DEBUG CHUNK" <<SEP << "Key : " << it->first << std::endl;
		std::cout << "Value : " << it->second << std::endl;
		std::cout << "State : " << _chunked_state[it->first] << SEP << std::endl;
	}
	return 0;
}

std::string RequestHandler::pop_chunked(std::string _key)
{
	std::map<std::string, std::string>::iterator it;
	std::map<std::string, int>::iterator it_state;
	std::string res;
	it = _chunk_request.find(_key);
	it_state = _chunked_state.find(_key);

	res = it->second;
	_chunk_request.erase(it);
	_chunked_state.erase(it_state);
	return res;
}

