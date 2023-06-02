#ifndef PARSE_HPP
#define PARSE_HPP

#include "Server.hpp"
#include <unistd.h>
#include <vector>
//isalnum
#include <ctype.h>

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



void parse(std::string recv, Client *cli, Server &serv);
void ft_ping(std::vector<std::string> &recv_vector, Client *cli, Server &serv);
void ft_pass(std::vector<std::string> &recv_vector, Client *cli, Server &serv);
void ft_nick(std::vector<std::string> &recv_vector, Client *cli, Server &serv);
void ft_name(std::vector<std::string> &recv_vector, Client *cli, Server &serv);
void ft_privmsg(std::vector<std::string> &recv_vector, Client *cli, Server &serv);
void ft_topic(std::vector<std::string> &recv_vector, Client *cli, Server &serv);
void ft_join(std::vector<std::string> &recv_vector, Client *cli, Server &serv);
void ft_mode(std::vector<std::string> &recv_vector, Client *cli, Server &serv);
void ft_kick(std::vector<std::string> &recv_vector, Client *cli, Server &serv);
void ft_quit(std::vector<std::string> &recv_vector, Client *cli, Server &serv);
void ft_invite(std::vector<std::string> &recv_vector, Client *cli, Server &serv);

#endif
