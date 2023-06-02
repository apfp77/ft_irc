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
		std::map<std::string, int> parse_map;

	public:
		std::vector<Client *>cli_vector;
		Server(char *srv_port, char *passwd);
		void set_socket(int socker_fd);
		int get_socket() const;
		Channel* find_ch_with_ch_name(std::string &channel_name);
		void set_channel(std::string &channel_name);
		int get_cmd(std::string s);
		char* get_passwd() const;
		
		void insert_cli(Client *cli);
		void delete_cli(Client *cli);
		
		void insert_ch(Channel *ch);
		void delete_ch(Channel *ch);

		
		Client* find_cli_with_nick_name(std::string &nick_name);


		~Server();

};

#endif
