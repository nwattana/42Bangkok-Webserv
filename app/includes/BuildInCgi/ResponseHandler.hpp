#pragma once
#include <string>
#include "utils.hpp"
#include <map>
#include "Response.hpp"


class ResponseHandler {
	private:

		std::string _version;
		std::map<int, std::string> _error_page;
		std::string index_page;

	public:
		ResponseHandler();
		ResponseHandler(int status_code, std::string body);
		~ResponseHandler();
		ResponseHandler(ResponseHandler const &src);
		ResponseHandler &operator=(ResponseHandler const &rhs);

		Response generate_response(int status_code, std::string body);
		void add_error_page(std::string path, int status_code);

		std::string generate_error_response(int status_code);
};
