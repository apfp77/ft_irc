#include <iostream>
#include "parse.hpp"
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <exception>
#include <signal.h>

#include <cctype>

#define ERROR 1

#define MAX_CLIENTS 10
#include <unistd.h>

bool program_down = false;

void signal_handle(int sig_num)
{
	if (sig_num == SIGINT || sig_num == SIGQUIT)
	{
		program_down = true;
		return ;
	}
}

int main(int argc, char **argv)
{
	signal(SIGINT, signal_handle);
	signal(SIGTERM, signal_handle);
	signal(SIGQUIT, signal_handle);
	if (argc != 3)
	{
		std::cout << "input ./ircserv <port> <password>" << '\n';
		return (ERROR);
	}
	while (true)
	{
		try
		{
			Server server(argv[1], argv[2]);
			server.parse_map_init();
			server.init();
			server.make_event_window();
			server.execute();
		}
		catch (int e)
		{
			std::cout << "EXIT" << '\n';
			return (0);
		}
		catch (const char *str)
		{
			std::cout << "ERROR: " << str << "\nerrno: " << errno << std::endl;
		}
		catch (std::exception &e)
		{
			std::cout << e.what() << '\n';
		}
	}
}
