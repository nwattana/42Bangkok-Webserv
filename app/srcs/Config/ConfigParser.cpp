#include "ConfigParser.hpp"

bool IsConfFile(const std::string &filename)
{
	if (filename.length() < 5)
		return false;
	std::string extension = filename.substr(filename.length() - 5);
	return extension == ".conf";
}

ConfigParser::ConfigParser(std::string filename) : m_fileName(filename)
{
	this->server_bracket = 0;
	this->location_bracket = 0;
	this->_isSetLocation = 0;
	this->_isSetServer = 0;
	if (!IsConfFile(m_fileName))
	{
		// error msg goes here
		exitWithError("Wrong extension (must be .conf)", 1, 1);
	}
	try
	{
		this->read_file_config();
		// this->get_server_config();
	}
	catch (std::exception &e)
	{
		std::cerr << "Error Occured " << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}
}

ConfigParser::~ConfigParser()
{
}

/// @brief Default constructor private member fucntion cant be called
ConfigParser::ConfigParser() : m_fileName("srcs/webserv.conf")
{
	std::string filename = "nginx.conf";
	if (!IsConfFile(filename))
	{
		// error msg goes here
		exitWithError("Wrong extension (must be .conf)", 1, 1);
	}
	this->read_file_config();
}

ConfigParser::ConfigParser(ConfigParser const &src)
{
	*this = src;
}

ConfigParser &ConfigParser::operator=(ConfigParser const &rhs)
{
	if (this != &rhs)
	{
		// copy
	}
	return (*this);
}

/// @brief Read Config file and keep this m_raw_config
void ConfigParser::read_file_config(void)
{
	std::string line;
	std::vector<std::string> splited;
	std::vector<std::string>::iterator it;
	std::string line_trim;

	m_file.open(m_fileName.c_str());
	if (!m_file.is_open())
	{
		// error msg goes here
		exitWithError("Error opening file", 1, 1);
	}
	while (std::getline(m_file, line))
	{
		line_trim = ft_strtrim(line);
		if (line_trim[0] == '#' || line_trim.empty())
			continue;
		splited = ft_split(line_trim, "\t\r\n\v\f 	");
		it = splited.begin();

		while (it != splited.end())
		{
			if (this->is_allow_directive(*it) == SERVER)
			{
				this->_isSetServer = SET;
			}
			else if (this->_isSetServer && this->_isSetLocation == NOTSET)
			{
				this->processServerBlock(it);
			}
			else if (this->_isSetLocation == SET)
			{
				this->processLocationBlock(it);
			}
			else
			{
				std::cout << "Error config token : " << *it << std::endl;
				// std::cout << this->_isSetLocation << " " << this->_isSetServer << std::endl;
			}

			it++;
		}
		// TODO: maybe have bug here
		tmp_server.endDirective();
		tmp_location.endDirective();
		this->m_raw_config += line_trim;
		this->m_raw_config += '\n';
	}
	m_file.close();
}

void ConfigParser::printServerConfig(void) const
{
	for (size_t i = 0; i < this->server_config.size(); i++)
	{
		std::cout << "Server " << i << std::endl;
		this->server_config[i].printServerConfig();
	}
}

// check top level directive
int ConfigParser::is_allow_directive(std::string str)
{
	if (str.compare("server") == 0)
	{
		return (SERVER);
	}
	if (str.compare("server{") == 0)
	{
		this->server_bracket += 1;
		return (SERVER);
	}
	return (0);
}

void ConfigParser::processLocationBlock(std::vector<std::string>::iterator &it)
{
	// LOCATION HERE
	if (tmp_location.isSetLocationMatch() == NOTSET)
	{
		if (tmp_location.isAcceptedPath(*it) == 0)
			throw std::invalid_argument("Invalid location path");
		tmp_location.setLocationMatch(*it);
	}
	else if (*it == "{")
	{
		tmp_location.openConfig();
	}
	else if (*it == "}")
	{
		tmp_location.closeConfig();
		tmp_server.addLocationBlock(tmp_location);
		tmp_location = LocationBlock();
		this->_isSetLocation = NOTSET;
	}
	else if (tmp_location.isSetDirective())
	{
		tmp_location.checkDirective(*it);
	}
	else if (tmp_location.isSetDirectiveArgument())
	{
		tmp_location.setDirectiveArgument(*it);
	}
}

void ConfigParser::processServerBlock(std::vector<std::string>::iterator &it)
{
	if (tmp_server.isGetDirective() && tmp_server.isAllowDirective(*it))
	{
		tmp_server.toSetDirective(*it);
	}
	else if (this->_isSetLocation == NOTSET && *it == "location")
	{
		this->_isSetLocation = SET;
	}
	else if (tmp_server.isGetArgument())
	{
		tmp_server.setDirectiveArgument(*it);
	}
	else if (*it == "{" || *it == "}")
	{
		handleBrackets(*it);
	}
	else
	{
		std::cout << "Error token :| " << *it << " | " << std::endl;
	}
}

void ConfigParser::handleBrackets(const std::string &token)
{
	if (token == "{")
	{
		this->server_bracket += 1;
	}
	else if (token == "}")
	{
		this->server_bracket -= 1;
	}
	if (this->server_bracket == 0)
	{
		this->_isSetServer = 0;
		tmp_server.closeConfig();
		this->server_config.push_back(tmp_server);
		tmp_server = ServerConfig();
	}
}

std::vector<ServerConfig> ConfigParser::getServerConfig(void) const
{
	return (this->server_config);
}
