#include "LocationBlock.hpp"
#include "Webserv.hpp"

LocationBlock::LocationBlock()
{
	this->allow_directive.push_back("root");
	this->allow_directive.push_back("index");
	this->allow_directive.push_back("cgi_file");
	this->allow_directive.push_back("cgi_pass");
	this->allow_directive.push_back("method_allow");

	// init empty vector to each key
	for (size_t i = 0; i < 5; i++)
	{
		this->location_configs[this->allow_directive[i]] = std::vector<std::string>();
	}
	this->_isSetDirective = NOTSET;
	this->_isSetDirectiveArgument = NOTSET;
	this->_isSetLocationMatch = NOTSET;
	this->_isEndConfig = NOTSET;
	this->_isOpenConfig = NOTSET;
}

LocationBlock::~LocationBlock()
{
}

LocationBlock::LocationBlock(LocationBlock const &src)
{
	*this = src;
}

LocationBlock &LocationBlock::operator=(LocationBlock const &rhs)
{
	if (this != &rhs)
	{
		this->_location_match = rhs._location_match;
		this->_isSetLocationMatch = rhs._isSetLocationMatch;
		this->_isSetDirective = rhs._isSetDirective;
		this->_isSetDirectiveArgument = rhs._isSetDirectiveArgument;
		this->_isEndConfig = rhs._isEndConfig;
		this->_isOpenConfig = rhs._isOpenConfig;
		this->_currentDirective = rhs._currentDirective;
		this->location_configs = rhs.location_configs;
	}
	return (*this);
}

int LocationBlock::isAcceptedPath(std::string path)
{
	char *c = (char *)path.c_str();
	for (size_t i = 0; i < path.length(); i++)
	{
		if ((isalnum(c[i]) || c[i] == '/' || c[i] == '_') == 0)
			return (0);
	}
	return (1);
}

void LocationBlock::printConfig(void) const
{
	std::map<std::string, std::vector<std::string> >::const_iterator it;
	std::vector<std::string> value;

	std::cout << "\tLocation : \"" << this->_location_match << "\"" << std::endl;
	for (it = this->location_configs.begin(); it != this->location_configs.end(); it++)
	{
		std::cout << "\t\t" <<  it->first << " : ";
		value = it->second;
		printStringVector(value, ", ");
	}
}

int LocationBlock::setLocationMatch(std::string location_match)
{
	this->_location_match = location_match;
	this->_isSetLocationMatch = SET;
	return (SUCCESS);
}

int LocationBlock::isSetLocationMatch(void)
{
	return (this->_isSetLocationMatch == SET);
}

int LocationBlock::openConfig(void)
{
	if (this->_isSetLocationMatch == NOTSET)
		throw std::runtime_error("Location match not set");
	this->_isOpenConfig = SET;
	this->_isSetDirective = SET;
	return (SUCCESS);
}

int LocationBlock::closeConfig(void)
{
	this->_isEndConfig = SET;
	this->_isOpenConfig = NOTSET;
	return (SUCCESS);
}

int LocationBlock::isSetDirective(void)
{
	return this->_isSetDirective == SET;
}

int LocationBlock::checkDirective(std::string directive_str)
{
	for (size_t i = 0; i < 5; i++)
	{
		if (this->allow_directive[i] == directive_str)
		{
			this->_currentDirective = directive_str;
			this->_isSetDirectiveArgument = SET;
			this->_isSetDirective = NOTSET;
			return (SUCCESS);
		}
	}
	throw std::runtime_error("Invalid directive");
}

int LocationBlock::isSetDirectiveArgument(void)
{
	return this->_isSetDirectiveArgument == SET;
}

int LocationBlock::setDirectiveArgument(std::string directive_argument)
{
	this->location_configs[this->_currentDirective].push_back(directive_argument);
	this->_isSetDirectiveArgument = NOTSET;
	return (SUCCESS);
}

int LocationBlock::endDirective(void)
{
	this->_isSetDirective = SET;
	this->_currentDirective = std::string();
	return (SUCCESS);
}
