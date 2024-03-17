#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
// #include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>


#define PORT "8080"   // Port we're listening on

int get_listener_socket(void)
{
	struct addrinfo hints;
	int listener;
	int yes;
	int rv;

	
}

int main(void)
{
	int listener; //listening socket descriptor

	int newdfd; //newly accept()ed socket descriptor
	struct sockaddr_storage remoteaddr; //client address
	socklen_t addrlen;

	char buf[256]; //buffer for client data

	char remoteIP[INET6_ADDRSTRLEN];

	int fd_count = 0;
	int fd_size = 5;
	struct pollfd *fds = malloc(sizeof(struct pollfd) * fd_size);

	listener = get_listener_socket();
}