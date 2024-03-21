#include "RequestHandler.hpp"


RequestHandler::RequestHandler()
{
	_allowed_method = GET | POST;
}

RequestHandler::RequestHandler(ServerConfig serverConfig)
{
	// set allowed method
	std::string config_in = serverConfig.getConfig(S_METHOD_ALLOW)[0];
	if (config_in.empty())
		_allowed_method = GET | POST;
	else
		_allowed_method = allow_method_parser(config_in);

	// set root dir
	_root_dir = serverConfig.getConfig(S_ROOT)[0];
	if (_root_dir.empty())
		_root_dir = "./assets/";

	std::vector<LocationBlock> location_configs = serverConfig.getLocationConfig();
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
}

int RequestHandler::check_allowed_method(int method)
{
	return (method & _allowed_method);
}

int RequestHandler::read_request(std::string request)
{
	std::stringstream ss;
	std::string methode, uri, version;

	// Raw Request
	// std::cout << "====================" << std::endl;
	// std::cout << "RequestHandler::read_request" << std::endl;
	// std::cout << "====================" << std::endl;
	// std::cout << "Request : " << request << std::endl;
	// std::cout << "====================" << std::endl;

	// seperate header and body
	int seperator = request.find("\r\n\r\n");
	if (seperator == -1)
	{
		std::cerr << "RequestHandler::read_request : no header/body seperator" << std::endl;
		return -1;
	}

	std::string header = request.substr(0, seperator + 2);
	std::string body = request.substr(seperator + 4);
	// Create Request object
	Request req(header, body);
	req.printSetting();

	return 0;
}

void RequestHandler::create_location_rule(std::vector<LocationBlock> location_config)
{
	LocationRule *rule;
	// for each location block, create a location rule
	for (std::vector<LocationBlock>::iterator it = location_config.begin(); it != location_config.end(); it++)
	{
		std::cout << "LocationBlock : " << std::endl;
		rule = new LocationRule(*it, _root_dir);
		_locations_rules.push_back(rule);
	}
	// sort location rules by depth
}

void RequestHandler::printSetting(void) const
{
	std::cout << "Allowed Method : ";
	method_allow_to_string(_allowed_method);
	std::cout << "Root Dir : " << _root_dir << std::endl;
	for (std::vector<LocationRule *>::const_iterator it = _locations_rules.begin(); it != _locations_rules.end(); it++)
	{
		(*it)->printSetting();
	}
}

