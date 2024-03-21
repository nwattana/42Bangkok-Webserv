#pragma once
#include <iostream>
#include "LocationBlock.hpp"
#include "ServerConfig.hpp"
#include "Constant.hpp"
#include "utils.hpp"
#include "exception.hpp"

class LocationRule{
	public:
		LocationRule();
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

		void setRootDir(std::string root_dir);
		std::string getRootDir(void) const;

		void setCGIBin(std::string cgi_bin);
		std::string getCGIBin(void) const;

		void setUploadPath(std::string upload_path);
		std::string getUploadPath(void) const;

		void setAllowMethod(int allow_method);
		int getAllowMethod(void) const;

		
		std::string get_handle_route(void) const;
		std::string get_full_path(void) const;


	private:

		void _set_up_default();

		std::string _handle_route;
		std::string _full_path;
		// std::string _dir_path;
		std::string _root_dir;
		std::string _cgi_bin;
		int _depth;
		int _allow_method;
		int _allow_upload;
		int _cgi_handle;
		std::string _upload_path;

		void _set_handle_route(LocationBlock block);
		// void _set_dir_path(LocationBlock block, std::string server_dir);
		void _set_root_dir(LocationBlock block, std::string server_dir);
		void _set_cgi_bin(LocationBlock block, std::string server_dir);

		void _set_allow_method(LocationBlock block);
		void _set_upload(LocationBlock block);
};
