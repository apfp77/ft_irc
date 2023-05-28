#include "Client.hpp"


Client::Client()
{
	pass_flag = false;
}

int Client::get_socket() const
{
	return (this->cli_sock);
}

void Client::set_socket(int cli_sock)
{ this->cli_sock = cli_sock; }
