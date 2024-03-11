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
		int isAllowDirective(std::string str);
		int isCloseConfig(void);
		int isEndDirective(void);
		int isSetLocation(void);
		int toSetDirective(std::string str);
		int setDirectiveArgument(std::string str);
		int closeConfig(void);
		int endDirective(void);
		int addLocationBlock(LocationBlock location);

	private:
		std::vector<std::string> allow_directive;
		std::map<std::string, std::vector<std::string> > configs;
		std::vector<LocationBlock> location_config;
		std::string current_set_directive;

		short int is_error;
		short int arguement_count;
		short int _isSetDirective;
		short int _isDoneConfig;
		short int _isSetLocation;
		short int _isCloseConfig;
		short int _isEndDirective;
};
