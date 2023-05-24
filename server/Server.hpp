#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <Channel.hpp>
#include <string>
#include <map>

class Server {
	private:
		int socket_fd;
		int port;
		char *passwd;
		std::map<std::string, Channel *> map;

	public:
		Server(char *port, char *passwd);
		void set_socket(int socker_fd);
		int get_socket() const;

		~Server();
};

#endif
