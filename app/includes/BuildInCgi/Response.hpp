#pragma once
#include <string>
#include "utils.hpp"

class Response{

	private:

		std::string _version;
		int _status_code;

		std::string _status_line;
		std::string _header_set;
		std::string _body;

	public:
		Response();
		Response(int status_code, std::string body);
		~Response();
		Response(Response const &src);
		Response &operator=(Response const &rhs);

		std::string getResponse();
		void setStatusCode(int status_code);
		void setBody(std::string body);
		void setHeader(std::string key, std::string value);
};