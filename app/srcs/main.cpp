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


	// RequestHandler requestHandler(config.getServerConfig()[0]);

	// requestHandler.read_request(
	// 	"GET / HTTP/1.1\r\n"
	// 	"Host: localhost:8080\r\n"
	// 	"User-Agent: curl/7.68.0\r\n"
	// 	"Accept: */*\r\n"
	// 	"\r\n"
	// );
	// ServerManager serverManager(config.getServerConfig());
	// serverManager.setupServer();
	// serverManager.handleConnection();
	// serverManager.closeAll();

	return (0);
}

