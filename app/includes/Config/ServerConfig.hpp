#pragma once
#include "LocationBlock.hpp"
#include <vector>
#include <iostream>
#include <string>

class ServerConfig
{
	public:
		ServerConfig(void);
		~ServerConfig(void);
		ServerConfig(ServerConfig const &src);
		ServerConfig &operator=(ServerConfig const &rhs);

		// SETUP
		void printServerConfig(void) const;
		int isGetArgument(void);
		int isGetDirective(void);
		int isCloseConfig(void);
		int isEndDirective(void);
		int isSetLocation(void);
		int toSetDirective(int directive);
		int setDirectiveArgument(std::string str);
		int closeConfig(void);
		int endDirective(void);
		int addLocationBlock(LocationBlock location);

		int check_directive(std::string str);

		//pls just let me get config info easily TT
		std::vector<std::string> getConfig(int directive);
		// std::string get_directive_string(int directive);

	private:
		// std::vector<std::string> allow_directive;
		std::map<int, std::vector<std::string> > configs;
		std::vector<LocationBlock> location_config;
		int current_set_directive;

		short int is_error;
		short int argument_count;
		short int _isSetDirective;
		short int _isDoneConfig;
		short int _isSetLocation;
		short int _isCloseConfig;
		short int _isEndDirective;
};

