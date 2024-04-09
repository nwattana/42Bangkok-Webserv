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
#include "ServerManager.hpp"
#include "RequestHandler.hpp"
#include "utils.hpp"

#include <signal.h>

#ifndef EXIT_FAILURE
#define EXIT_FAILURE 1
#endif

static ConfigParser *g_config;
static ServerManager *g_serverManager;


#endif
