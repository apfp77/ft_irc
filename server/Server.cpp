#include "Server.hpp"
//stringstream
#include <sstream>

Server::Server(char *port, char *passwd) : passwd(passwd)
{
	std::stringstream ss;
	ss >> this->port;
}

void Server::set_socket(int socket_fd)
{
	this->socket_fd = socket_fd;
}

int Server::get_socket() const
{
	return (this->socket_fd);
}