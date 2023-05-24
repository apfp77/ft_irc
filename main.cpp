#include <iostream>
#include "Server.hpp"
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

//isdigit
#include <cctype>

#define ERROR 1

bool is_num(char *s)
{
	int i = 0;
	if (s[i] == '-' || !std::isdigit(s[i++]))
		return (false);
	while (s[i] && std::isdigit(s[i]))
		i++;
	if (s[i] == '\0')
		return (true);
	return (false);
}

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cout << "input ./ircserv <port> <password>" << '\n';
		return (ERROR);
	}
	if (!is_num(argv[1]))
	{
		std::cout << "input positive integer" << '\n';
		return (ERROR);
	}
	Server server(argv[1], argv[2]);
	server.set_socket(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP));
	if (server.get_socket() < 0)
	{
		std::cout << "socket error" << '\n';
		return (1);
	}
	


	return 0;
}
