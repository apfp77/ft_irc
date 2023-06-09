#include "Client.hpp"


Client::Client() 
{ 
	pass_flag = false;
	nick_flag = false;
	nick_name = "";
	user_name = "";
}

void Client::set_fds_index(int fds_index){ this->fds_index = fds_index; }

int Client::get_fds_index() const{ return (this->fds_index); }


struct sockaddr_in &Client::get_cil_addr()
{
	return (this->cli_sockaddr);
}

socklen_t &Client::get_cli_size()
{
	return (this->cli_sockaddr_l);
}

int Client::append_submemory(char *str)
{
	int idx;

	this->sub_memory.append(str);
	idx = sub_memory.rfind("\r\n");
	return (idx);
}

std::string Client::division_cmd(int idx)
{
	std::string cmd_memory;

	cmd_memory = this->sub_memory.substr(0, idx + 2);
	this->sub_memory = this->sub_memory.substr(idx + 2);
	return (cmd_memory);
}

void Client::setting_socket()
{
	int value = 1;
	if (setsockopt(this->cli_sock, SOL_SOCKET, SO_REUSEADDR, (const void *)&value, sizeof(value)) == -1)
		throw ("setsoctopt() error");
	if (fcntl(this->cli_sock, F_SETFL, O_NONBLOCK) == -1)
		throw ("fcntl() error");
}

socklen_t cli_sockaddr_l;

int Client::get_socket() const { return (this->cli_sock); }

void Client::set_socket(int cli_sock) { this->cli_sock = cli_sock; }

void Client::set_nick_name(std::string &nick_name) { this->nick_name = nick_name; }

std::string Client::get_nick_name() const { return (this->nick_name); }

Client::~Client()
{
	close(this->cli_sock);
}