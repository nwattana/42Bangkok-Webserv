#pragma once
#include <iostream>
#include <linux/limits.h>
#include <unistd.h>
#include "LocationBlock.hpp"
#include "ServerConfig.hpp"
#include "Constant.hpp"
#include "utils.hpp"
#include "Exception.hpp"
#include "Request.hpp"
#include "ResponseHandler.hpp"
#include "ExecvAdapter.hpp"
#include "StaticFileWaiter.hpp"
#include "FileCreator.hpp"
#include "Response.hpp"

#ifndef LOCATION_RULE_DEBUG
# define LOCATION_RULE_DEBUG 0
#endif


/*
 * convert route to full path
*/
#define ERROR_RESOURCE "error:f57e9322ebe34807a0de7cf52dc52288"

class LocationRule{
	public:
		LocationRule();
		LocationRule(LocationBlock& block, std::string server_dir, std::string server_name, std::string server_port);
		LocationRule(LocationRule const &src);
		LocationRule &operator=(LocationRule const &rhs);
		~LocationRule();


		// Compare Rule Depth
		bool operator>(LocationRule const &rhs);
		bool operator<(LocationRule const &rhs);
		bool operator==(LocationRule const &rhs);

		int get_depth(void) const;

		void setIndexPage(std::string index_page, int defalt);

		std::string set_auto_index(std::string auto_index);
		std::string set_auto_index(LocationBlock& block);

		int getAllowMethod(void) const;
		//////////////////////////////////////////// initialize
		void setRootDir(std::string root_dir);
		void setCGIBin(std::string cgi_bin);
		void setUploadPath(std::string upload_path);
		void setAllowMethod(int allow_method);
		int setAllowMethod(std::vector<std::string> allow_method);

		////////////////////////////////////////////// check request object
		bool match_uri_path(std::string uri) const;

		////////////////////////////////////////////// execute request
		std::string execute_request(Request& request);
		std::string cgi_call(Request& request);
		std::string cgi_parse_url(Request& request);

		std::string location_generate_simple_response(int error_code);

		////////////////////////////////////////////// DEBUG delete able
		void printSetting(void) const;
		void location_print_error_page(void) const;
		

		////////////////////////////////////////////// Public Getter
		std::string get_handle_route(void) const;
		std::string get_index_page(void) const;
		std::string getRootDir(void) const;
		std::string getCGIBin(void) const;
		std::string getUploadPath(void) const;


		////////////////////////////////////////////// Public Setter
		void set_host_name(std::string host_name);
		void set_server_port(std::string server_port);
		void set_root_dir(std::string root_dir);
		void set_method_allow(std::vector<std::string> allow_method);
		

		////////////////////////////////////////////// will Move to build in cgi and extenal cpp cgi
		std::string generate_resource_GET(std::string uri);
		std::string generate_resource_POST(std::string uri);
		std::string generate_resource_DELETE(std::string uri);
		// std::string generate_response(Request * srequest);
		int _handle_upload(Request &requset);
		int _handle_delete_file(Request &request);

		//////////////////////////////////////////////  handle error response
		void set_error_page(std::map<int, std::string> error_page);

	private:
		int _is_set_index_page;
		int is_set_cgi_script;
		
		int _depth;
		std::string _handle_route;
		std::string _root_dir;
		int _cgi_handle;
		
		std::string _cgi_script;

		int _allow_method;
		int _allow_upload;

		std::string _upload_path;
		std::string _index_page;
		std::string _auto_index;
		std::string _auto_index_cgi;
		std::string _host_name;
		std::string _server_port;

		std::string _cwd;
		std::map<std::string, std::string> _chunk_request;
		////////////////////////////////////////////// initialize
		void _set_handle_route(LocationBlock block);
		// void _set_dir_path(LocationBlock block, std::string server_dir);
		void _set_up_default();
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


		////////////////////////////////////////////// move to build in cgi 
		std::string _handle_get(Request& request);
		std::string _handle_post(Request& request);
		std::string _handle_put(Request& request);
		std::string _handle_delete(Request& request);
		std::string _build_in_cgi(Request& request);
		ResponseHandler *_m_response_handler;
		std::map<int, std::string> _error_page;
};
