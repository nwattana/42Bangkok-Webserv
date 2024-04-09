#include "Webserv.hpp"


static ConfigParser *g_config;
static ServerManager *g_serverManager;


void signalHandler(int signum)
{
	std::cout << "Interrupt signal (" << signum << ") received.\n";
	if (g_serverManager)
		delete g_serverManager;
	if (g_config)
		delete g_config;
	exit(signum);
}

int main(int argc, char **argv)
{
	g_config = NULL;
	g_serverManager = NULL;
	//set filename
	if (argc > 2)
	{
		//error msg goes here
		return (1);
	}
	std::string filename = (argc == 2) ? argv[1] : "webserv.conf";

	signal(SIGINT, signalHandler);

	// ConfigParser config(filename);
	g_config = new ConfigParser(filename);
	if (SERVERCONFIG_DEBUG)
	{
		db_red("=============== Initializing server from: " + filename + " =============== ");
		// config.printServerConfig();
		g_config->printServerConfig();
	}
	db_red("=============== Initializing server from: " + filename + " =============== ");

	// test_http_request_split_line();
	// ServerManager serverManager(config.getServerConfig());
	// serverManager.setupServer();
	// serverManager.handleConnection();
	// serverManager.closeAll();
	g_serverManager = new ServerManager(g_config->getServerConfig());
	g_serverManager->setupServer();
	g_serverManager->handleConnection();
	g_serverManager->closeAll();
	delete g_serverManager;
	delete g_config;
	return (0);
}

