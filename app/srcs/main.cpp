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
	std::cout  << "=============== Initializing server from: " << filename << " =============== " << std::endl;
	config.printServerConfig();
	std::cout << "=============== Connecting to http://localhost:8080/ ===============" << std::endl;
	//create server

	// creat_server_v1();
	// create_server_v2();
	// Server server;
	Server server(config.getServerConfig()[0]);
	server.connectServer();
	return (0);
}

