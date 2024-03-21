#pragma once
#include <iostream>
#include "LocationBlock.hpp"
#include "Constant.hpp"
#include "utils.hpp"

class LocationRule{
	public:
		LocationRule(LocationBlock block, std::string server_dir);
		LocationRule(LocationRule const &src);
		LocationRule &operator=(LocationRule const &rhs);
		~LocationRule();

		void printSetting(void) const;

		bool operator>(LocationRule const &rhs);
		bool operator<(LocationRule const &rhs);
		bool operator==(LocationRule const &rhs);

		int get_depth(void) const;
		bool match_uri_path(std::string uri) const;


	private:
		LocationRule();

		void _set_up_default();

		std::string _full_path;
		std::string _handle_route;
		std::string _dir_path;
		std::string _root_dir;
		std::string _cgi_bin;
		int _depth;
		int _allow_method;
		int _allow_upload;
		int _cgi;
		std::string _upload_path;
};
