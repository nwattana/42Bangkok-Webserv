
#include "Webserv.hpp"

int creat_server_v1(void)
{
		int server_fd;
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		exitWithError("Error: unable to create a socket", 1);
	}

	sockaddr_in sockaddr;
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(8081);
	sockaddr.sin_addr.s_addr = INADDR_ANY;
	int res;
	if ((res = bind(server_fd, (struct sockaddr *)&sockaddr, sizeof(sockaddr))) < 0)
	{
		std::cout << "res: " << res << std::endl;
		exitWithError("Error: unable to bind to port 9000", 1);
	}
	if ((listen(server_fd, 3)) < 0) {
		exitWithError("Error: unable to listen", 1);
	}

	int acceptedSocket;
	struct sockaddr_storage their_addr;
	socklen_t addr_size = sizeof(their_addr);
	if ((acceptedSocket = accept(server_fd, (struct sockaddr *)&sockaddr, &addr_size)) < 0)
	{
		std::cout << "acceptedSocket: " << acceptedSocket << std::endl;
		exitWithError("Error: unable to accept connection", 1);
	}
	std::cout << "Connection accepted" << std::endl;

	int bufferSize = 30000;
	char buffer[bufferSize];
	int byteRead;
	std::cout << "Message from client: " << std::endl;
	if ((byteRead = read(acceptedSocket, buffer, bufferSize)) > 0) {
		std::cout << buffer;
	}
	if (byteRead < 0) {
		exitWithError("Error: unable to read from socket", 1);
	}

	std::string response = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
	if (write(acceptedSocket, response.c_str(), response.length()) < 0) {
		exitWithError("Error: unable to write to socket", 1);
	}

	if (close(acceptedSocket))
	{
		std::cout << "Error: unable to close acceptedSocket" << std::endl;
	}

	if (close(server_fd))
	{
		std::cout << "Error: unable to close server_fd" << std::endl;
	}
	std::cout << std::endl << "=============== Conncetion closed Successfully ===============" << std::endl;
	return (0);
}