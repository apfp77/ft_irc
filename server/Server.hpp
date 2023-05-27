#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <Channel.hpp>
#include <string>
#include <map>

#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

class Server {
	private:
		int server_socket_fd;
		int port;
		char *passwd;
		/*
			채널 이름과 채널포인터
			map["채널명"]으로 접속하기 위함
		*/
		std::map<std::string, Channel *> map;

	public:
		std::vector<Client *>cli_vector;
		Server(char *port, char *passwd);
		void set_socket(int socker_fd);
		int get_socket() const;
		Channel *get_channel(std::string &channel_name);
		void set_channel(std::string &channel_name);
		~Server();
};

#endif
