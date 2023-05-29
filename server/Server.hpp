#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <Channel.hpp>
#include <string>
#include <set>
#include <map>

// ㅌㅔ스트용
#include <vector>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

class Server {
	private:
		int srv_sock;
		int srv_port;
		char *passwd;
		struct sockaddr_in serv_addr;
		std::set<Channel *> ch_set;
		std::set<Client *> cli_set;
		std::map<const char *, int> parse_map;

	public:
		std::vector<Client *>cli_vector;
		Server(char *srv_port, char *passwd);
		void set_socket(int socker_fd);
		int get_socket() const;
		Channel *get_channel(std::string &channel_name);
		void set_channel(std::string &channel_name);
		int get_cmd(const char *s);
		~Server();
};

#endif
