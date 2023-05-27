#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <poll.h>
#include <sys/socket.h>
#include <netinet/in.h>


class Client {
	private:
		/*
			nick_name 중복체크
		*/
		std::string nick_name;
		std::string user_name;
		int socket_fd;


	public:
		struct sockaddr_in cli_sockaddr;
		socklen_t cli_sockaddr_l;
		int get_socket() const;
		void set_socket(int socket_fd);
		Client();
		~Client();
};

#endif
