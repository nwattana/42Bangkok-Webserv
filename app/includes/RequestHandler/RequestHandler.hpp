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
	int read_request(std::string request);

	void create_location_rule(std::vector<LocationBlock> location_config);
	void printSetting(void) const;

	void read_requst(std::string request);

private:

	LocationRule *_find_location_rule(std::string uri);
	int _request_serializer(Request &request);
	int _allowed_method;
	std::vector<LocationRule *> _locations_rules;
	std::string _root_dir;
};
