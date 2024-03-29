#pragma once
#include "Constant.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <vector>
#include "exception.hpp"

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
	int endDirective(void);

	int check_directive(std::string directive);
	int set_current_directive(std::string directive);
	std::string get_directive_str(int directive) const;

	std::vector<std::string> getConfig(int directive) const;
	std::string getLocationMatch(void) const;

	void setErrorPage(void);
	std::map<int, std::string> getErrorPage(void) const;
	void print_error_page(void) const;

private:
	std::string _location_match;
	std::map<int, std::vector<std::string> > location_configs;
	std::map<int, std::string> error_page;

	int _isSetDirective;
	int _isSetDirectiveArgument;
	int _isSetLocationMatch;
	int _isEndConfig;
	int _isOpenConfig;
	int _currentDirective;
};
