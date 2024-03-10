#include "ServerConfig.hpp"
#include "Webserv.hpp"

ServerConfig::ServerConfig()
	: allow_directive{"index", "listen", "server_name", "root", "method_allow", "directory_list", "client_max_size", "error_page"},
	  current_set_directive(""),
	  is_error(0),
	  _isSetDirective(SET),
	  _isDoneConfig(NOTSET),
	  _isCloseConfig(NOTSET),
	  _isEndDirective(NOTSET)
{
}

ServerConfig::~ServerConfig()
{
}

ServerConfig::ServerConfig(ServerConfig const &src)
{
	*this = src;
}

/// @brief assign operator
ServerConfig &ServerConfig::operator=(ServerConfig const &rhs)
{
	if (this != &rhs)
	{
		this->allow_directive = rhs.allow_directive;
		this->configs = rhs.configs;
		this->current_set_directive = rhs.current_set_directive;
		this->is_error = rhs.is_error;
		this->_isSetDirective = rhs._isSetDirective;
		this->_isDoneConfig = rhs._isDoneConfig;
		this->_isCloseConfig = rhs._isCloseConfig;
		this->_isEndDirective = rhs._isEndDirective;
		this->location_config = rhs.location_config;
	}
	return (*this);
}

/// @brief print server config for debug purpose
void ServerConfig::printServerConfig() const
{
    std::vector<std::string> value;
    std::cout << "Server Configs" << std::endl;
    for (std::vector<std::string>::const_iterator it = this->allow_directive.begin(); it != this->allow_directive.end(); ++it)
    {
        std::cout << *it << " : ";
        value = this->configs.find(*it)->second;
        printStringVector(value, ", ");
    }
    for (std::vector<LocationBlock>::const_iterator it = this->location_config.begin(); it != this->location_config.end(); ++it)
    {
        it->printConfig();
    }
}

/// @brief  check if current directive is set STATE to recieve new argument
int ServerConfig::isGetArgument()
{
	return (this->arguement_count > 0);
}

/// @brief check if current directive is set STATE to recieve new directive
int ServerConfig::isGetDirective()
{
	return (this->_isSetDirective == SET);
}

int ServerConfig::isAllowDirective(std::string str)
{
	for (size_t i = 0; i < this->allow_directive.size(); i++)
	{
		if (str.compare(this->allow_directive[i]) == 0)
		{
			return (1);
		}
	}
	return (0);
}

int ServerConfig::toSetDirective(std::string str)
{
	this->current_set_directive = str;
	this->_isSetDirective = 0;
	this->arguement_count = 1;
	return (1);
}

int ServerConfig::setDirectiveArgument(std::string str)
{
	std::string temp(str);

	this->configs[this->current_set_directive].push_back(str);
	this->_isSetDirective = SET;
	return (1);
}

/// @brief Set the Server Config to Done State
int ServerConfig::closeConfig(void)
{
	// this->printServerConfig();
	// TODO Check parsed completly set and all valid
	_isDoneConfig = SET;
	return (1);
}

/// @brief Check if the Server Config is Done
int ServerConfig::isCloseConfig()
{
	return (this->_isCloseConfig == SET);
}

/// @brief clear state get argument of current directive for recieving new directive
int ServerConfig::endDirective()
{
	this->_isSetDirective = SET;
	this->arguement_count = 0;
	{
		this->current_set_directive = std::string();
		this->_isSetDirective = SET;
	}
	return (1);
}

/// @brief check is current directive is done
int ServerConfig::isEndDirective()
{
	// TODO if close before done should throw error
	return (this->_isEndDirective == SET);
}

/// @brief check is current state working on location block
int ServerConfig::isSetLocation()
{
	return (this->_isSetLocation == SET);
}

/// @brief add location block to server config
int ServerConfig::addLocationBlock(LocationBlock location)
{
	this->location_config.push_back(location);
	return (1);
}