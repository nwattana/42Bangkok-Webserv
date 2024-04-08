#include "utils.hpp"

/// @brief Check if a socket is still alive by peeking at the socket
/// @param sockfd The socket file descriptor to check
/// @return 0 if the socket is no longer alive, 1 if the socket is still alive -1 if an error occurred
int check_sockfd_alive(int sockfd)
{
	int try_recv;
	char buff[1];

	try_recv = recv(sockfd, buff, 1, MSG_PEEK);
	if (try_recv == -1)
	{
		if (errno == ECONNRESET)
		{
			db_red("Socket is no longer alive (connection reset by peer).\n");
		}
		else
		{
			perror("recv");
		}
	}
	else if (try_recv == 0)
	{
		db_red("Socket is no longer alive (connection closed by peer).\n");
	}
	else
	{
		db_green("Socket is still alive.\n");
	}
	return (try_recv);
}