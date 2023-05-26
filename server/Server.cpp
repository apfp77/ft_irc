#include "Server.hpp"
//stringstream
#include <sstream>

Server::Server(char *port, char *passwd) : passwd(passwd)
{
	std::stringstream ss;
	ss >> this->port;
}

void Server::set_socket(int server_socket_fd)
{
	this->server_socket_fd = server_socket_fd;
}

int Server::get_socket() const
{
	return (this->server_socket_fd);
}