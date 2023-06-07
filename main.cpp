#include <iostream>
#include "parse.hpp"
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <exception>
#include <signal.h>

//isdigit
#include <cctype>

#define ERROR 1

#define MAX_CLIENTS 10
#include <unistd.h>


// void signal_handle(int sig_num)
// {
// 	if (sig_num == SIGINT || sig_num == SIGQUIT)
// 		return ;
// }

int main(int argc, char **argv)
{
	// signal(SIGINT, signal_handle);
	// signal(SIGTERM, signal_handle);
	// signal(SIGQUIT, signal_handle);
	if (argc != 3)
	{
		std::cout << "input ./ircserv <port> <password>" << '\n';
		return (ERROR);
	}
	try
	{
		Server server(argv[1], argv[2]);
		server.parse_map_init();
		server.init();
		server.make_event_window();
		server.execute();
	}
	catch (int a)
	{
		std::cout << "ERROR: " << a << std::endl;
	}
}
	
