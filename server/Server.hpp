#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <Channel.hpp>
#include <map>

class Server {
	private:
		int socket_fd;
		std::map<std::string, Channel *> map;

	public:
		Server();
		~Server();
};

#endif
