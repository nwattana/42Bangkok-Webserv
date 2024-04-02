#pragma once
#include <string>
#include <cstdio>
#include "ServerConfig.hpp"
#include <sstream>
#include "Constant.hpp"
#include "utils.hpp"
#include "LocationRule.hpp"
#include "Request.hpp"
#include "Constant.hpp"
#include "exception.hpp"

#define SEP "\n=======================================\n"

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

	void create_location_rule(std::vector<LocationBlock>& location_config);
	void create_defualt_rule(void);
	void printSetting(void) const;

	std::string generate_error_response(std::string route, int error_code);
private:
	LocationRule *_find_location_rule(std::string uri);
	int number_called;

	LocationRule *_default_rule;
	std::string _request_serializer(void);
	int _allowed_method;
	std::vector<LocationRule *> _locations_rules;
	std::string _root_dir;

	std::map<std::string, std::string> _chunk_request;
	std::map<std::string, int> _chunked_state;

	Request *_current_request;
	std::string add_or_append_chunked(std::string _key, std::string _body);
	short get_chunked_state(std::string _key);
	std::string pop_chunked(std::string _key);
	short debug_chunked_state(void);

};
