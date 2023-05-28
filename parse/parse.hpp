#ifndef PARSE_HPP
#define PARSE_HPP

#include "Server.hpp"

#define PING 1
#define PASS 2
#define NICK 3
#define NAME 4
#define PRIVMSG 5
#define TOPIC 6
#define JOIN 7
#define MODE 8
#define KICK 9



void parse(std::string recv, Client *cli, Server &serv);

#endif
