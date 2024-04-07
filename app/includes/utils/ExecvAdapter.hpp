#pragma once
#include <map>
#include "utils.hpp"

#ifndef EXECV_ADAPTER_LOG
# define EXECV_ADAPTER_LOG 0
#endif

class ExecveAdapter
{
	private:
		int argv_size;
		char **argv;
		int envp_size;
		char **envp;
		std::map<std::string, std::string> _env_map;
		ExecveAdapter();

	public:
		ExecveAdapter(std::string path, std::vector<std::string> env_list);
		~ExecveAdapter();

		char **get_argv();
		char **get_envp();
		int get_argv_size();
		int get_envp_size();

		void destroy_argv();
		void destroy_envp();
		void destory();

		int add_env(std::string key, std::string value);
		int add_env(std::string env_string);

		char **get_envp_array();
};

// Test this module
void test_ExecveAdapter(void);

