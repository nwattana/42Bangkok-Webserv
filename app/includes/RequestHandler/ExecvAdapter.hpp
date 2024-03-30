#pragma once
#include "utils.hpp"

class ExecveAdapter
{
	private:
		int argv_size;
		char **argv;
		int envp_size;
		char **envp;
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
};

// Test this module
void test_ExecveAdapter(void);

