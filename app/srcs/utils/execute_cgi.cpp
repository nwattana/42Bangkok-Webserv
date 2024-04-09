#include "Webserv.hpp"

std::string execute_cgi(int argc, char *argv[], char *const *envp)
{
	int fd[2];
	pipe(fd);
	std::string result;
	struct timeval tv;
	int status;
	
	db_blue("DO EXECUTE FILE : " + std::string(YELLOW) + argv[0]);

	pid_t c_pid = fork();
	if (c_pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (c_pid == 0)
	{								
		close(fd[0]);				
		dup2(fd[1], STDOUT_FILENO);
		int res = execve(argv[0], argv, envp);
		free_char_array(argv);
		free_char_array((char **)envp);
		alarm(1);
		if (res == -1)
		{
			perror("execve");
			exit(1);
		}
		close(fd[1]);
		exit(1);
	}
	else
	{				
		if (WIFSIGNALED(status))
		{
			if (WTERMSIG(status) == SIGALRM) {
				std::cerr << "Timeout reached. The child process took too long to execute." << RESET << std::endl;
				return "2980f025-918e-42dc-a0bc-843452f04847-timeout"
			}
		} 
		close(fd[1]);
		waitpid(c_pid, &status, 0);
		// set equal max size 3MB dont let cgi set more than 3MB lol
		char buffer[3000000];
		memset(buffer, 0, sizeof(buffer));
		read(fd[0], buffer, sizeof(buffer));
		result = buffer;
		close(fd[0]);
	}
	// std::cout<< MAGENTA << result << std::endl;
	return result;
}

// just for test purpose
void test_use_execute_cgi()
{
	char *args[2];
	args[0] = strdup("./cgi-bin/py/hello_world.py");
	args[1] = NULL;

	char *argvs[2];
	argvs[0] = strdup("REQUEST_METHOD=GET");
	argvs[1] = NULL;

	execute_cgi(0, args, argvs);
}
