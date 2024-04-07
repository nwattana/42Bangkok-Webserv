#include "Webserv.hpp"

int main(int argc, char **argv, char **envp)
{
	//set filename
	if (argc > 2)
	{
		//error msg goes here
		return (1);
	}
	std::string filename = (argc == 2) ? argv[1] : "webserv.conf";

	ConfigParser config(filename);
	if (SERVERCONFIG_DEBUG)
	{
		db_red("=============== Initializing server from: " + filename + " =============== ");
		config.printServerConfig();
	}
	db_red("=============== Initializing server from: " + filename + " =============== ");

	// test_http_request_split_line();
	ServerManager serverManager(config.getServerConfig());
	serverManager.setupServer();
	serverManager.handleConnection();
	serverManager.closeAll();
	return (0);
}

