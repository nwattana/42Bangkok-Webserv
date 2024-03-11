#pragma once
#include "Constant.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <vector>

class LocationBlock
{
public:
	LocationBlock(void);
	~LocationBlock(void);

	LocationBlock(LocationBlock const &src);
	LocationBlock &operator=(LocationBlock const &rhs);


	void printConfig(void) const;

	// SETUP
	int setLocationMatch(std::string location_match);
	int openConfig(void);
	int closeConfig(void);
	int isSetDirectiveArgument(void);
	int setDirectiveArgument(std::string argument);
	int isSetLocationMatch(void);
	int isAcceptedPath(std::string path);
	int isSetDirective(void);
	int checkDirective(std::string directive);
	int endDirective(void);

private:
	std::string _location_match;
	std::vector<std::string> allow_directive;
	std::map<std::string, std::vector<std::string> > location_configs;

	int _isSetDirective;
	int _isSetDirectiveArgument;
	int _isSetLocationMatch;
	int _isEndConfig;
	int _isOpenConfig;
	std::string _currentDirective;
};

// std::ostream &operator<<(std::ostream &os, const LocationBlock &rhs);
