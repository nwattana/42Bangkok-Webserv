#pragma once
#include <iostream>
#include <linux/limits.h>
#include <unistd.h>
#include "LocationBlock.hpp"
#include "ServerConfig.hpp"
#include "Constant.hpp"
#include "utils.hpp"
#include "exception.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "ResponseHandler.hpp"
#include "ExecvAdapter.hpp"
#include "StaticFileWaiter.hpp"
#include "FileCreator.hpp"

#ifndef LOCATION_RULE_DEBUG
# define LOCATION_RULE_DEBUG 0
#endif

class LocationRule{
	public:
		LocationRule();
		LocationRule(LocationBlock& block, std::string server_dir);
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
		std::string generate_response(Request * srequest);
		std::string get_index_page(void) const;
		void setIndexPage(std::string index_page, int defalt);

		std::string generate_resource_GET(std::string uri);
		std::string generate_resource_POST(std::string uri);
		void set_error_page(std::map<int, std::string> error_page);
		void print_error_page(void) const;

		// response handler
		std::string generate_error_response(int error_code);
		std::string execute_request(Request& request);
		std::string set_auto_index(std::string auto_index);
		std::string set_auto_index(LocationBlock& block);
	
	private:

		void _set_up_default();
		std::string _full_path;
		int _is_set_index_page;
		int _depth;
		std::string _handle_route;
		std::string _root_dir;
		int _cgi_handle;
		std::string _cgi_bin;
		int _allow_method;
		int _allow_upload;
		std::string _upload_path;
		std::string _index_page;
		std::string _auto_index;
		std::string _auto_index_cgi;
		std::string _host_name;
		std::string _server_port;

		void _set_handle_route(LocationBlock block);
		// void _set_dir_path(LocationBlock block, std::string server_dir);
		void _set_root_dir(LocationBlock block, std::string server_dir);
		void _set_cgi_bin(LocationBlock block, std::string server_dir);
		void _set_depth();
		void _set_allow_method(LocationBlock block);
		void _set_upload(LocationBlock block);
		void _set_cgi(LocationBlock block, std::string server_dir);
		void _set_index_page(LocationBlock block);
		void _set_error_page(std::map<int, std::string> error_page);
		void _set_cwd(void);

		void _add_index_page(std::string &uri);

		void _set_host_name(std::string host_name);
		void _set_server_port(std::string server_port);

		std::string _handle_get(Request& request);
		std::string _handle_post(Request& request);
		std::string _handle_put(Request& request);
		std::string _handle_delete(Request& request);

		std::string _build_in_cgi(Request& request);

		ResponseHandler *_m_response_handler;
		std::map<int, std::string> _error_page;
		std::string _cwd;
	
		std::map<std::string, std::string> _chunk_request;
};
