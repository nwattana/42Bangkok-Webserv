#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include "Webserv.hpp"
#include "ServerConfig.hpp"
#include "LocationBlock.hpp"

#define OPEN_BRACKET 1
#define CLOSE_BRACKET 0

class ConfigParser
{
public:
	ConfigParser(std::string filename);
	~ConfigParser();
	ConfigParser(ConfigParser const &src);
	ConfigParser &operator=(ConfigParser const &rhs);

	void printServerConfig(void) const;
	std::vector<ServerConfig> getServerConfig(void) const;
private:
	ConfigParser();

	int _isSetServer;
	int _isSetLocation;
	int server_bracket;
	int location_bracket;
	// std::map<std::string, std::vector<std::string> > root_config;
	// std::vector<std::string> allow_directive;
	std::vector<ServerConfig> server_config;
	std::string m_fileName;
	std::string m_raw_config;
	std::fstream m_file;
	std::stringstream ss;
	ServerConfig tmp_server;
	LocationBlock tmp_location;

	int is_allow_directive(std::string str);
	void processLocationBlock(std::string token);
	void processServerBlock(std::string token);
	void handleBrackets(const std::string& token);
	void read_file_config(void);
};

bool IsConfFile(const std::string &filename);

#endif
