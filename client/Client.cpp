#include "Client.hpp"


Client::Client(){}

int Client::get_socket() const
{
	return (this->socket_fd);
}

void Client::set_socket(int socket_fd)
{ this->socket_fd = socket_fd; }
