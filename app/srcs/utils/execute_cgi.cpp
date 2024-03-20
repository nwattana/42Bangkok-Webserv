#include "Webserv.hpp"

std::string execute_cgi(int argc, char *argv[], char *const *envp)
{
	int fd[2];
	pipe(fd);
	std::string result;

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
		char *args[] = {"./cgi-bin/hello_world.py", NULL};
		execve(args[0], args, envp);
		close(fd[1]);
		exit(1);
	}
	else
	{				 
		close(fd[1]);
		waitpid(c_pid, NULL, 0);
		// set equal max size 3MB dont let cgi set more than 3MB lol
		char buffer[3000000];
		memset(buffer, 0, sizeof(buffer));
		read(fd[0], buffer, sizeof(buffer));
		result = buffer;
		close(fd[0]);
	}
	return result;
}