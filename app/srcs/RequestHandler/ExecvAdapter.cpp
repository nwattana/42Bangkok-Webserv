#include "ExecvAdapter.hpp"
#include "utils.hpp"

ExecveAdapter::ExecveAdapter()
{
	argv = NULL;
	envp = NULL;
}


ExecveAdapter::ExecveAdapter(std::string path, std::vector<std::string> env_list)
{
	char **temp;
	argv = ft_vector_string_to_char_array(ft_split(path, " "));
	
	temp = argv;
	while (*temp)
	{
		argv_size++;
		temp++;
	}
	envp = NULL;
	
	std::vector<std::string>::iterator it = env_list.begin();
	for (it = env_list.begin(); it != env_list.end(); it++)
	{
		// std::string key = *it;
		std::pair<std::string, std::string> pair = ft_split_pair(*it, "=");
		_env_map.insert(pair);
	}
}

ExecveAdapter::~ExecveAdapter()
{
	// destroy_argv();
	// destroy_envp();
}


char **ExecveAdapter::get_argv()
{
	return argv;
}

char **ExecveAdapter::get_envp()
{
	return envp;
}

void ExecveAdapter::destroy_argv()
{
	free_char_array(argv);
}

void ExecveAdapter::destroy_envp()
{
	free_char_array(envp);
}

int ExecveAdapter::get_argv_size()
{
	return argv_size;
}

int ExecveAdapter::get_envp_size()
{
	return envp_size;
}

void ExecveAdapter::destory()
{
	destroy_argv();
	argv = NULL;
	destroy_envp();
	envp = NULL;
}

// Just For Testing
void test_ExecveAdapter(void)
{
	std::string path = "./cgi-bin/py/hello_world.py";
	std::vector<std::string> env_list;
	env_list.push_back("REQUEST_METHOD=GET");
	ExecveAdapter adapter(path, env_list);
	char **argv = adapter.get_argv();
	char **envp = adapter.get_envp();
	while (*argv)
	{
		std::cout << *argv << std::endl;
		argv++;
	}
	while (*envp)
	{
		std::cout << *envp << std::endl;
		envp++;
	}
	adapter.destroy_argv();
	adapter.destroy_envp();
}

int ExecveAdapter::add_env(std::string key, std::string value)
{
	std::pair<std::string, std::string> pair(key, value);
	_env_map.insert(pair);
	return 0;
}

int ExecveAdapter::add_env(std::string env_string)
{
	std::pair<std::string, std::string> pair = ft_split_pair(env_string, "=");
	_env_map.insert(pair);
	return 0;
}

char ** ExecveAdapter::get_envp_array(void)
{
	std::vector<std::string> env_list;
	std::map<std::string, std::string>::iterator it;

	for (it = _env_map.begin(); it != _env_map.end(); it++)
	{
		std::string env = it->first + "=" + it->second;
		env_list.push_back(env);
	}
	envp = ft_vector_string_to_char_array(env_list);
	return envp;
}