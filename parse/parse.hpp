
#ifndef PARSE_HPP
#define PARSE_HPP

#include "Server.hpp"
#include <unistd.h>
#include <vector>

#include <ctype.h>
#include "Numerics.hpp"
#include "ft_utils.hpp"

#define PING 1
#define PASS 2
#define NICK 3
#define NAMES 4
#define PRIVMSG 5
#define TOPIC 6
#define JOIN 7
#define MODE 8
#define KICK 9
#define QUIT 10
#define INVITE 11
#define CAP 12
#define PART 13



void parse(std::string recv, Client *cli, Server &serv);
void ft_names(std::vector<std::string> &recv_vector, Client *cli, Server &serv);
void ft_ping(std::vector<std::string> &recv_vector, Client *cli, Server &serv);
bool ft_pass(std::vector<std::string> &recv_vector, Client *cli, Server &serv);
void ft_nick(std::vector<std::string> &recv_vector, Client *cli, Server &serv);
bool ft_connect_nick(std::vector<std::string> &recv_vector, Client *cli, Server &serv);
void ft_names(std::vector<std::string> &recv_vector, Client *cli, Server &serv);
void ft_privmsg(std::vector<std::string> &recv_vector, Client *cli, Server &serv);
void ft_topic(std::vector<std::string> &recv_vector, Client *cli, Server &serv);
void ft_join(std::vector<std::string> &recv_vector, Client *cli, Server &serv);
void ft_mode(std::vector<std::string> &recv_vector, Client *cli, Server &serv);
void ft_kick(std::vector<std::string> &recv_vector, Client *cli, Server &serv);
void ft_quit(Client *cli);
void ft_invite(std::vector<std::string> &recv_vector, Client *cli, Server &serv);
void ft_part(std::vector<std::string> &recv_vector, Client *cli, Server &serv);
void ft_cap();
/*
	names
*/
void ret_join_after_names_message(Channel *ch, Client *cli);


#endif
