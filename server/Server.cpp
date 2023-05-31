#include "Server.hpp"
//stringstream
#include <sstream>

#include <cstdlib>

Server::Server(char *srv_port, char *passwd) : passwd(passwd)
{
	parse_map["PING"] = 1;
	parse_map["PASS"] = 2;
	parse_map["NICK"] = 3;
	parse_map["NAME"] = 4;
	parse_map["PRIVMSG"] = 5;
	parse_map["TOPIC"] = 6;
	parse_map["JOIN"] = 7;
	parse_map["MODE"] = 8;
	parse_map["KICK"] = 9;

	std::stringstream ss(srv_port);
	ss >> this->srv_port;
}

void Server::set_socket(int srv_sock){	this->srv_sock = srv_sock; }

int Server::get_socket() const{ 	return (this->srv_sock); }

int Server::get_cmd(std::string s) {	return (this->parse_map[s]); }

char* Server::get_passwd() const { return (this->passwd); }

Client* Server::find_cli_with_nick_name(std::string &nick_name)
{
	std::set<Client *>::iterator it = this->cli_set.begin();
	for (; it != cli_set.end(); it++)
	{
		if(!nick_name.compare((*it)->get_nick_name()))
		{
			return ((*it));
		}
	}
	return (NULL);
}

Channel* Server::get_channel(std::string &channel_name)
{
	std::set<Channel *>::iterator it = this->ch_set.begin();
	for (; it != this->ch_set.end(); ++it)
	{
		if ((*it)->get_ch_name() == channel_name)
			return ((*it));
	}
	return (NULL);
}

Server::~Server(){}