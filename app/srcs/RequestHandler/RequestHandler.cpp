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
		this->_default_rule->setIndexPage(serverConfig.getConfig(S_INDEX)[0]);
	}
	catch (ConfigNotFoundException &e)
	{
		std::cerr << "RequestHandler::RequestHandler : " << e.what() << std::endl;
		this->_default_rule->setIndexPage("index.html");
	}

	// Other location rules
	std::vector<LocationBlock>& location_configs = serverConfig.getLocationConfig();
	create_location_rule(location_configs);
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
	std::cout << "RequestHandler::~RequestHandler" << std::endl;
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

	uri = _current_request->get_uri();
	LocationRule *rule = _find_location_rule(uri);
	if (rule == NULL)
	{
		rule = _default_rule;
	}
	std::string response = rule->execute_request(*_current_request);
	// Clean up
	delete _current_request;

	std::cout << "RequestHandler::read_request : " << number_called << std::endl;
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