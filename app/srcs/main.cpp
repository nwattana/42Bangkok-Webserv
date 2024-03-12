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
	if (res = (bind(server_fd, (struct sockaddr *)&sockaddr, sizeof(sockaddr))) < 0)
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

#include <arpa/inet.h>
void printAddressInfo(struct addrinfo *p)
{
	char ipstr[INET6_ADDRSTRLEN];
	void *addr;
	std::string ipver;
	struct addrinfo *serverinfo = p; // will point to the results

	while (serverinfo != NULL)
	{

		// get the pointer to the address itself,
		// different fields in IPv4 and IPv6:
		if (serverinfo->ai_family == AF_INET) { // IPv4
			struct sockaddr_in *ipv4 = (struct sockaddr_in *)serverinfo->ai_addr;
			addr = &(ipv4->sin_addr);
			ipver="IPv4";
		}
		else { // IPv6
			struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)serverinfo->ai_addr;
			addr = &(ipv6->sin6_addr);
			ipver="IPv6";
		}

		// convert the IP to a string and print it:
		inet_ntop(serverinfo->ai_family, addr, ipstr, sizeof(ipstr));
		std::cout << "  " << ipver << ": " << ipstr << std::endl;

		// show content in struct
		std::cout << "  ai_family: " << serverinfo->ai_family << std::endl;
		std::cout << "  ai_socktype: " << serverinfo->ai_socktype << std::endl;
		std::cout << "  ai_protocol: " << serverinfo->ai_protocol << std::endl;
		std::cout << "  ai_addrlen: " << serverinfo->ai_addrlen << std::endl;
		// std::cout << "  ai_canonname: " << serverinfo->ai_canonname << std::endl;
		std::cout << "  ai_addr: " << serverinfo->ai_addr << std::endl;
		std::cout << "  ai_next: " << serverinfo->ai_next << std::endl;
		serverinfo = serverinfo->ai_next;
	}
}

// ใช้ Recv และ Send แทน Read และ Write
int create_server_v2(void)
{
	int status;
	struct addrinfo hints;
	struct addrinfo *servinfo; // result
	std::string str_port = "8080";
	char *port = (char *)str_port.c_str();
	std::string error;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC; // use AF_INET6 to force IPv6
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // use my IP

	if ((status = getaddrinfo("www.example.com", port, &hints, &servinfo)) != 0)
	{
		exitWithError("Error: getaddrinfo", 1);
	}
	printAddressInfo(servinfo);

	// create a socket
	int sockfd;
	sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
	if (sockfd < 0)
	{
		exitWithError(strerror(errno), 1);
	}

	// bind the socket
	struct sockaddr_in my_addr;
	my_addr.sin_family = AF_INET;
	// Check for port length
	my_addr.sin_port = htons(atoi(port));     // short, network byte order
	my_addr.sin_addr.s_addr = INADDR_ANY;
	memset(my_addr.sin_zero, '\0', sizeof my_addr.sin_zero);
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &my_addr, sizeof(my_addr)) == -1)
	{
		exitWithError("Error: setsockopt", 1);
	}

	if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(my_addr)) == -1)
	{
		std::cout << "Bind" << std::endl;
		error = strerror(errno);
		exitWithError(error, 1);
	}
	else{
		std::cout << "Bind to port " << port  << std::endl;
	}

	if (listen(sockfd, 10) == -1)
	{
		exitWithError("Error: listen", 1);
	}

	struct sockaddr_storage their_addr;
	socklen_t addr_size = sizeof(their_addr);
	int accept_socket = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);
	if (accept_socket == -1)
	{
		exitWithError("Error: accept", 1);
	}

	std::cout << "Connection accepted from fd : " << accept_socket << std::endl;
	char buffer[30000];

	if (recv(accept_socket, &buffer, 29999, 0) < 0)
	{
		std::cout << strerror(errno) << std::endl;
		exitWithError("Error: recv", 1);
	}
	std::cout << "Message from client: " << buffer << std::endl;

	std::string response = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
	if (send(accept_socket, response.c_str(), response.length(), 0) < 0){
		exitWithError("Error: unable to write to socket", 1);
	}
	else {
		getpeername(accept_socket, (struct sockaddr *)&their_addr, &addr_size);
		std::cout << "Connection from: " << inet_ntoa(((struct sockaddr_in *)&their_addr)->sin_addr) << std::endl;
	}

	char hostname[1024];
	memset(hostname, 0, 1024);
	gethostname(hostname, 1023);
	std::cout << "Hostname: " << hostname << std::endl;

	close(sockfd);
	close(accept_socket);
	freeaddrinfo(servinfo);
	return (0);
}

int main(int argc, char **argv)
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
	create_server_v2();
	return (0);
}

