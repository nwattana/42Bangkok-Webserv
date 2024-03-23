#pragma once
#include <string>
#include <cstdio>
#include "ServerConfig.hpp"
#include <sstream>
#include "Constant.hpp"
#include "utils.hpp"
#include "LocationRule.hpp"
#include "Request.hpp"

/*
 * Factory class Crete RequestHandler for Server
 * each Server will have a RequestHandler
 * GET, POST, PUT, DELETE, HEAD, OPTIONS, TRACE, CONNECT,
 */

class RequestHandler
{
public:
	RequestHandler();
	RequestHandler(ServerConfig serverConfig);
	RequestHandler(RequestHandler const &src);
	RequestHandler &operator=(RequestHandler const &rhs);
	~RequestHandler();

	int check_allowed_method(int method);
	std::string read_request(std::string request);

	void create_location_rule(std::vector<LocationBlock> location_config);
	void create_defualt_rule(void);
	void printSetting(void) const;

	void read_requst(std::string request);
	std::string generate_error_response(std::string route, int error_code);

private:
	LocationRule *_find_location_rule(std::string uri);

	LocationRule *_default_rule;
	std::string _request_serializer(void);
	int _allowed_method;
	std::vector<LocationRule *> _locations_rules;
	std::string _root_dir;

	Request *_current_request;
};
