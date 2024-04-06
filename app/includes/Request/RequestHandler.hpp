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
#include "Exception.hpp"
#include "ChunkData.hpp"

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
	RequestHandler(ServerConfig &serverConfig);
	RequestHandler(RequestHandler const &src);
	RequestHandler &operator=(RequestHandler const &rhs);
	~RequestHandler();

	std::string read_request(std::string request);
	// std::string read_request(char *buffer, size_t size);

	// Initialize RequestHandler
	void set_root_dir(ServerConfig &serverConfig);
	void create_defualt_rule(ServerConfig &serverConfig);
	void create_location_rule(std::vector<LocationBlock> &location_config);

	// for server call
	std::string generate_simple_response(std::string route, int error_code);

	// Parsing and Create Request Object
	// std::string handle_chunked_request(void);
	std::string create_request_object(std::string request);

	// Search for Location Rule

	// Let Rule Handle Request

	// DEBUG
	// void printSetting(void) const;
	// short debug_chunked_state(void);

private:
	std::string _root_dir;
	std::string _server_name;
	std::string _server_port;

	int number_called;
	Request *_current_request;

	// Search for Location Rule
	LocationRule _default_rule;
	std::vector<LocationRule *> _locations_rules;
	LocationRule *_find_location_rule(std::string uri);

	// std::string _request_serializer(void);

	// Contain pennding chunked request
	std::map<std::string, std::vector<ChunkData*> > _chunk_request;
	// check chunk_request state
	std::map<std::string, int> _chunked_state;
};
