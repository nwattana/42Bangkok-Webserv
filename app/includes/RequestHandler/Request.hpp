#pragma once
#include <string>
#include <cstdio>
#include <iostream>
#include <map>
#include <vector>
#include <sstream>
#include "utils.hpp"

/*
Request    = Request-Line
			*(( general-header
				| request-header
				| entity-header ) CRLF)
			CRLF
			[ message-body ]


Request-Line   = Method SP Request-URI SP HTTP-Version CRLF
Method         = "GET" | "POST" | "PUT" | "DELETE" | "HEAD"

# data structure
- `_header`: Request-Line + header + CRLF
- `_request_line`: Request-Line
- `_header_map`: header map
- `_body`: message-body
- `_method`: Method
- `_uri`: Request-URI
- `_version`: HTTP-Version
	
*/
class Request
{

public:
	Request(std::string header, std::string body);
	Request(Request const &src);
	Request &operator=(Request const &rhs);	
	~Request();

	void printSetting(void) const;
	void print_request_line_info(void) const;
	void print_header(void) const;
	void print_body(void) const;

	std::string get_request_line(void);
	std::string get_header(std::string key);
	std::string get_body(void);

	std::string raw_request(void);

private:
	Request();

	std::string _header;
	std::string _request_line;
	std::map<std::string, std::vector<std::string> > _header_map;
	std::string _body;
	int _method;
	std::string _uri;
	std::string _version;

	void _parse_request_line(std::string line);
	void _parse_header(std::string header);
};