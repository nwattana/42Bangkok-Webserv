#pragma once
#include <string>
#include "utils.hpp"

class ResponseHandler {

	private:

		std::string _version;
		int _status_code;

		std::string _status_line;
		std::string _header_set;
		std::string _body;

	public:
		ResponseHandler();
		ResponseHandler(int status_code, std::string body);
		~ResponseHandler();
		ResponseHandler(ResponseHandler const &src);
		ResponseHandler &operator=(ResponseHandler const &rhs);

		std::string getResponse();
		void setStatusCode(int status_code);
		void setBody(std::string body);
};


