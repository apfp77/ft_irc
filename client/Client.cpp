#include "Client.hpp"


Client::Client() 
{ 
	pass_flag = false;
	nick_name = "";
	user_name = "";
}

int Client::get_socket() const { return (this->cli_sock); }

void Client::set_socket(int cli_sock) { this->cli_sock = cli_sock; }

void Client::set_nick_name(std::string &nick_name) { this->nick_name = nick_name; }

std::string Client::get_nick_name() const { return (this->nick_name); }

Client::~Client(){}