#include "RequestHandler.hpp"

RequestHandler::RequestHandler()
{
	number_called = 0;
}

RequestHandler::RequestHandler(ServerConfig& serverConfig)
{
	// set allowed method
	number_called = 0;
	set_root_dir(serverConfig);

	// Other location rules
	create_defualt_rule(serverConfig);
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
	db_yellow("Warning!! RequestHandler::~RequestHandler");
	for (std::vector<LocationRule *>::iterator it = _locations_rules.begin(); it != _locations_rules.end(); it++)
	{
		delete *it;
	}
}

// SET ROOT dir
// SET SERVER NAME
// SET SERVER PORT
void RequestHandler::set_root_dir(ServerConfig& serverConfig)
{
	try {
		_root_dir = serverConfig.getConfig(S_ROOT)[0];
	}
	catch (ConfigNotFoundException &e)
	{
		std::cerr << YELLOW << e.what() << " set to default root dir " <<  DEFAULT_ROOT_DIR  << RESET << std::endl;
		_root_dir = DEFAULT_ROOT_DIR;
	}

	try {
		_server_name = serverConfig.getConfig(S_SERVER_NAME)[0];
	}
	catch (ConfigNotFoundException &e)
	{
		std::cerr << RED << e.what() << " Set To default server name " << DEFAULT_SERVER_NAME << RESET << std::endl;
		_server_name = DEFAULT_SERVER_NAME;
	}

	try {
		_server_port = serverConfig.getConfig(S_LISTEN)[0];
	}
	catch (ConfigNotFoundException &e)
	{
		std::cout << RED << e.what() << " Set to default Port " << DEFAULT_PORT << RESET << std::endl;
	}
}


// accept request and return response string
std::string RequestHandler::read_request(std::string request)
{
	std::string res;
	number_called++;

	db_blue("==================================New Request==========================\n");
	// std::cout << CYAN << "request "<< request << RESET << std::endl;
	res = create_request_object(request);
	if (res != "success")
	{
		return (res);
	}

	// call location rule to handle request
	std::string uri = _current_request->get_uri();
	LocationRule *rule = _find_location_rule(uri);
	if (rule == NULL)
	{
		rule = &_default_rule;
	}
	std::string response = rule->execute_request(*_current_request);
	delete _current_request;
	_current_request = NULL;
	db_blue("==================================/New Request " + std::string(YELLOW) + SSTR(number_called)+ " times ");
	return response;
}

/// @brief serialize request object from request string
/// return string "success" "partia"
std::string RequestHandler::create_request_object(std::string request)
{
	std::string header, body;
	std::string function_res = "test";
	// check first line 

	std::vector<std::string> vec = http_request_split_line(request);
	// เชค request line
	if (parse_request_line(vec[0]) == 1)
	{
		// มี request ตัดแบ่ง header กับ body
		int seperator = request.find("\r\n\r\n");
		if (seperator == -1)
		{
			db_red("Invalid Request missing seperator!");
			return _default_rule.location_generate_simple_response(400);
		}
		header = request.substr(0, seperator + 2);
		body = request.substr(seperator + 4);
		if (_current_request != NULL)
		{
			delete _current_request;
			_current_request = NULL;
		}
		_current_request = new Request(header, body);
	}
	else 
	{
		if (_current_request == NULL)
		{
			db_red("INVALID REQUEST LINE invalid Request 400");
			return (generate_simple_response("/", 400));
		}
		_current_request->append_body(request);
	}
	if (_current_request->check_if_multipart())
	{
		if (_current_request->check_chunk_done())
			return ("success");
		else
			return _default_rule.location_generate_simple_response(206);
	}
	return "success";
}

void RequestHandler::create_location_rule(std::vector<LocationBlock>& location_config)
{
	LocationRule *rule;
	// for each location block, create a location rule
	for (std::vector<LocationBlock>::iterator it = location_config.begin(); it != location_config.end(); it++)
	{
		rule = new LocationRule(*it, _root_dir, _server_name, _server_port);
		if (rule == NULL)
		{
			std::cerr << "RequestHandler::create_location_rule : failed to create location rule" << std::endl;
			continue;
		}
		rule->set_host_name(_server_name);
		rule->set_server_port(_server_port);
		_locations_rules.push_back(rule);
	}
}

LocationRule *RequestHandler::_find_location_rule(std::string uri)
{
	LocationRule *rule, *res;

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

void RequestHandler::create_defualt_rule(ServerConfig& serverConfig)
{
	// Set Root Dir from server config
	std::string temp;
	std::vector<std::string> temp_vec;

	// Set Root Directory for this rule
	try {
		temp = serverConfig.getConfig(S_ROOT)[0];
		_default_rule.set_root_dir(temp);
	}
	catch (ConfigNotFoundException &e)
	{
		// std::cerr << "RequestHandler::create_defualt_rule : " << e.what() << " set to default root dir " << DEFAULT_ROOT_DIR << std::endl;
		_default_rule.set_root_dir(DEFAULT_ROOT_DIR);
	}

	try {
		temp_vec = serverConfig.getConfig(S_METHOD_ALLOW);
		_default_rule.set_method_allow(temp_vec);
	}
	catch (ConfigNotFoundException &e)
	{
		// std::cerr << "RequestHandler::create_defualt_rule : " << e.what() << " set to default method allow only GET" << std::endl;
		temp_vec.push_back("GET");
		_default_rule.set_method_allow(temp_vec);
	}

	_default_rule.set_error_page(serverConfig.getErrorPage());

	try
	{
		_default_rule.setIndexPage(serverConfig.getConfig(S_INDEX)[0], 1);
	}
	catch (ConfigNotFoundException &e)
	{
		// std::cerr << "RequestHandler::RequestHandler : " << e.what() << " set to index.html for route \"/\"" << std::endl;
		_default_rule.setIndexPage("index.html", 0);
	}

	try
	{
		temp = serverConfig.getConfig(S_SERVER_NAME)[0];
		_default_rule.set_host_name(temp);
	}
	catch(ConfigNotFoundException &e)
	{
		// std::cerr << "RequestHandler::create_defualt_rule : " << e.what() << " set to default server name localhost" << std::endl;
		_default_rule.set_host_name(DEFAULT_SERVER_NAME);
	}

	try
	{
		temp = serverConfig.getConfig(S_LISTEN)[0];
		_default_rule.set_server_port(temp);
	}
	catch(ConfigNotFoundException &e)
	{
		// std::cerr << "RequestHandler::create_defualt_rule : " << e.what() << " set to default port 8080" << std::endl;
		_default_rule.set_server_port(DEFAULT_PORT);
	}
	
	try{
		std::string auto_index = serverConfig.getConfig(S_AUTO_INDEX)[0];
		if (auto_index == "enable")
			_default_rule.set_auto_index("enable");
	}
	catch (ConfigNotFoundException &e)
	{
		std::cout << YELLOW << e.what() << " auto-index/directory-list set to disable" << RESET << std::endl;
	}
}

/// @brief Serve a error response asoociated with the error code and route
/// @return
std::string RequestHandler::generate_simple_response(std::string route, int error_code)
{
	LocationRule *rule = _find_location_rule(route);
	if (rule == NULL)
	{
		rule = &_default_rule;
	}
	return rule->location_generate_simple_response(error_code);
}

/// @brief  DEBUG
void print_chunk_request(std::map<std::string, std::vector<ChunkData*> > data)
{
	std::map<std::string, std::vector<ChunkData*> >:: iterator it = data.begin();
	std::cout << (*it).first << std::endl;
	std::vector<ChunkData *> v=(*it).second;
	std::vector<ChunkData *>::iterator vit=v.begin();
	for (vit; vit != v.end(); vit++)
	{
		std::cout << (*vit)->get_data() << std::endl;
	}
}
