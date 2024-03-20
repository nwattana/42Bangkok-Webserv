#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <stdlib.h>
#include <cctype>
#include <sstream>
#include <map>
#include <vector>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>

#include "ConfigParser.hpp"
#include "Constant.hpp"
#include "Server.hpp"


#ifndef EXIT_FAILURE
#define EXIT_FAILURE 1
#endif


void						exitWithError(std::string errorMessage = "error", int mode = 0, int errnum = errno);
void 						printStringVector(std::vector<std::string> vec, std::string delimiter);
int 						findCharInString(std::string str, const char *c);
std::string 				ft_strtrim(std::string str);
std::vector<std::string> 	ft_split(std::string str, const char *delimiter);

std::string execute_cgi(int argc, char *argv[], char * const * envp);
#endif
