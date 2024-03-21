#include "LocationBlock.hpp"
#include "Webserv.hpp"

/* Location allow directive definition
 * root - the root directory of the server -> default is the current directory of the server
 * index - which file to serve if no file is specified in the request -> default is index.html
 * cgi_file - which file to execute if the request is a cgi request -> default is not set
 * cgi_pass - which file to execute if the request is a cgi request -> default is not set
 * method_allow - which methods are allowed -> default is GET, POST
 */
LocationBlock::LocationBlock()
{
	this->_isSetDirective = NOTSET;
	this->_isSetDirectiveArgument = NOTSET;
	this->_isSetLocationMatch = NOTSET;
	this->_isEndConfig = NOTSET;
	this->_isOpenConfig = NOTSET;
	this->_currentDirective = DEFAULT;
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
		// attributes
		this->_location_match = rhs._location_match;
		this->location_configs = rhs.location_configs;

		// state
		this->_isSetLocationMatch = rhs._isSetLocationMatch;
		this->_isSetDirective = rhs._isSetDirective;
		this->_isSetDirectiveArgument = rhs._isSetDirectiveArgument;
		this->_isEndConfig = rhs._isEndConfig;
		this->_isOpenConfig = rhs._isOpenConfig;
		this->_currentDirective = rhs._currentDirective;
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
	std::map<int, std::vector<std::string> >::const_iterator it;
	std::vector<std::string> value;

	std::cout << "\tLocation : \"" << this->_location_match << "\"" << std::endl;
	for (it = this->location_configs.begin(); it != this->location_configs.end(); it++)
	{
		std::cout << "\t\t" << this->get_directive_str(it->first) << " : ";
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

int LocationBlock::isSetDirectiveArgument(void)
{
	return this->_isSetDirectiveArgument == SET;
}

int LocationBlock::setDirectiveArgument(std::string directive_argument)
{
	this->location_configs[this->_currentDirective].push_back(directive_argument);
	return (SUCCESS);
}

int LocationBlock::endDirective(void)
{
	this->_isSetDirective = SET;
	this->_currentDirective = 0;
	this->_isSetDirectiveArgument = NOTSET;
	return (SUCCESS);
}

int LocationBlock::set_current_directive(std::string str)
{
	int directive = check_directive(str);
	if (directive == 0)
	{
		std::cout << "Error directive : " << directive << std::endl;
		throw std::invalid_argument("Invalid directive");
	}
	this->_currentDirective = directive;
	this->_isSetDirectiveArgument = SET;
	return (SUCCESS);
}

int LocationBlock::check_directive(std::string directive)
{

	if (directive == "method_allow")
		return (L_METHOD_ALLOW);
	else if (directive == "error_page")
		return (L_ERROR_PAGE);
	else if (directive == "upload_file")
		return (L_UPLOAD_FILE);
	else if (directive == "upload_path")
		return (L_UPLOAD_PATH);
	else if (directive == "root_dir")
		return (L_ROOT_DIR);
	else if (directive == "cgi_handle")
		return (L_CGI_HANDLER);
	else if (directive == "cgi_path")
		return (L_CGI_PATH);
	else if (directive == "auto_index")
		return (L_AUTOINDEX);
	return (0);
}

std::string LocationBlock::get_directive_str(int directive) const
{
	switch (directive)
	{
	case L_ROOT_DIR:
		return ("root_dir");
	case L_CGI_HANDLER:
		return ("cgi_handle");
	case L_CGI_PATH:
		return ("cgi_path");
	case L_DENY:
		return ("deny");
	case L_AUTOINDEX:
		return ("autoindex");
	case L_ERROR_PAGE:
		return ("error_page");
	case L_CLIENT_MAX_SIZE:
		return ("client_max_size");
	case L_UPLOAD_FILE:
		return ("upload_file");
	case L_UPLOAD_PATH:
		return ("upload_path");
	case L_METHOD_ALLOW:
		return ("method_allow");
	default:
		return ("");
	}
}

/**
 * @brief Retrieves the configuration for a given directive.
 * 
 * This function returns the configuration associated with the specified directive.
 * If the directive is found in the map of configurations, the corresponding vector
 * of strings is returned. Otherwise, a ConfigNotFoundException is thrown.
 * 
 * @param directive The directive to retrieve the configuration for.
 * @return The configuration associated with the directive.
 * @throws ConfigNotFoundException if the directive is not found.
 */
std::vector<std::string> LocationBlock::getConfig(int directive) const
{
	std::map<int, std::vector<std::string> >::const_iterator it;
	it = this->location_configs.find(directive);
	if (it == this->location_configs.end())
		throw ConfigNotFoundException("Config not found \"" + get_directive_str(directive) + "\"");
	return (it->second);
}

std::string LocationBlock::getLocationMatch(void) const
{
	return (this->_location_match);
}