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

#include <netinet/in.h>
#include <unistd.h>

#include "ConfigParser.hpp"
#include "Constant.hpp"
#include "ServerCluster.hpp"


#include <sys/types.h>
#include <netdb.h>

#ifndef EXIT_FAILURE
#define EXIT_FAILURE 1
#endif

#define SERVER 1
#define INDEX 2

void						exitWithError(std::string errorMessage = "error", int mode = 0, int errnum = errno);
void 						printStringVector(std::vector<std::string> vec, std::string delimiter);
int 						findCharInString(std::string str, const char *c);
std::string 				ft_strtrim(std::string str);
std::vector<std::string> 	ft_split(std::string str, const char *delimiter);
void printAddressInfo(struct addrinfo *p);
#endif
