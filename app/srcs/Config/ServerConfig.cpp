#include "ServerConfig.hpp"
#include "Webserv.hpp"


std::string get_directive_string(int directive);

/* SERVER CONFIG DIRECTIVE definition
- `index` - which file to serve if no file is specified in the request -> default is index.html
- `listen` - which port to listen on -> default is 8080
- `server_name` - the name of the server -> default is localhost
- `root` - a directory or a file from where the file should be searched -> default is the current directory of the server
- `method_allow` - which methods are allowed -> default is GET, POST
- `directory_list` - if directory listing is allowed -> default is off
- `client_max_size` - the maximum size of the client request -> default is 1000000 bytes
- `error_page` - which page to serve if an error occurs -> not set by default
- `upload_file` - which file upload is enable -> disble by default
- `upload_path` - where to upload the file -> not set by default
*/
ServerConfig::ServerConfig()
	: current_set_directive(0),
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
	std::map<int, std::vector<std::string> >::const_iterator it;
	std::cout << "Server Configs" << std::endl;

	for (it = this->configs.begin(); it != this->configs.end(); it++)
	{
		std::cout << "\t"<<get_directive_string(it->first) << ":\t";
		printStringVector(it->second, ",");

	}
	for (size_t i = 0; i < location_config.size(); i++)
	{
		location_config[i].printConfig();
	}

}

/// @brief  check if current directive is set STATE to recieve new argument
int ServerConfig::isGetArgument()
{
	return (this->argument_count > 0);
}

/// @brief check if current directive is set STATE to recieve new directive
int ServerConfig::isGetDirective()
{
	return (this->_isSetDirective == SET);
}

int ServerConfig::toSetDirective(int directive)
{
	this->current_set_directive = directive;
	this->_isSetDirective = 0;
	this->argument_count = 1;
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
	// check if default value is set is not set set default value
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
	this->argument_count = 0;
	{
		this->current_set_directive = 0;
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
	return (SET);
}

std::vector<std::string> ServerConfig::getConfig(int directive)
{
	std::map<int, std::vector<std::string> >::iterator it;
	try
	{
		it = this->configs.find(directive);
	}
	catch (std::exception &e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
	}
	if (it != this->configs.end())
		return it->second;
	throw std::runtime_error("Error: Directive not found");
}

int ServerConfig::check_directive(std::string str)
{
	if (str == "index")
	{
		return (S_INDEX);
	}
	else if (str == "listen")
	{
		return (S_LISTEN);
	}
	else if (str == "server_name")
	{
		return (S_SERVER_NAME);
	}
	else if (str == "root")
	{
		return (S_ROOT);
	}
	else if (str == "method_allow")
	{
		return (S_METHOD_ALLOW);
	}
	else if (str == "directory_list")
	{
		return (S_DIRECTORY_LIST);
	}
	else if (str == "client_max_size")
	{
		return (S_CLIENT_MAX_SIZE);
	}
	else if (str == "error_page")
	{
		return (S_ERROR_PAGE);
	}
	else if (str == "upload_file")
	{
		return (S_UPLOAD_FILE);
	}
	else if (str == "upload_path")
	{
		return (S_UPLOAD_PATH);
	}
	else
	{
		return (0);
	}
}

std::string get_directive_string(int directive)
{
	switch (directive)
	{
	case S_INDEX:
		return "index";
	case S_LISTEN:
		return "listen";
	case S_SERVER_NAME:
		return "server_name";
	case S_ROOT:
		return "root";
	case S_METHOD_ALLOW:
		return "method_allow";
	case S_DIRECTORY_LIST:
		return "directory_list";
	case S_CLIENT_MAX_SIZE:
		return "client_max_size";
	case S_ERROR_PAGE:
		return "error_page";
	case S_UPLOAD_FILE:
		return "upload_file";
	case S_UPLOAD_PATH:
		return "upload_path";
	default:
		return "unknown";
	}
}