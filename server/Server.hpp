#ifndef PAR_HPP
#define PAR_HPP
#include <parse.hpp>
#endif
#ifndef SERVER_HPP
#define SERVER_HPP
#include <iostream>
#include <Channel.hpp>
#include <string>
#include <set>
#include <map>
#include <poll.h>
#include <unistd.h>
#include <fcntl.h>

#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <cctype>
#include <cerrno>

#define MAXCLIENT 20
#define SERVER_NAME "FT_IRC "

class Server {
	private:
		int srv_sock;
		int srv_port;
		char *passwd;
		struct pollfd fds[MAXCLIENT + 1];
		struct sockaddr_in serv_addr;
		std::set<Channel *> ch_set;
		std::set<Client *> cli_set;
		std::map<std::string, int> parse_map;
		void accept_client();
		void erase_clinet(pollfd &fds);

	public:
		Server(char *srv_port, char *passwd);
		bool is_num(char *s);
		void parse_map_init();
		void init();
		void make_event_window();
		void execute();
		void setting_socket(int srv_sock);
		struct pollfd &find_vacant_fds();
		Client *find_client(int fd);
		void message_receive(pollfd &fds);
		void cli_belong_channel_delete(Client *cli);
		void set_socket(int socker_fd);
		int get_socket() const;
		void set_channel(std::string &channel_name);
		int get_cmd(std::string s);
		char* get_passwd() const;
		
		void insert_cli(Client *cli);
		void delete_cli(Client *cli);
		
		void insert_ch(Channel *ch);
		void delete_ch(Channel *ch);

		
		Channel* find_ch_with_ch_name(std::string &channel_name);
		Client* find_cli_with_nick_name(std::string &nick_name);
		std::set<Channel *> find_cli_in_channels(Client *cli);

		std::set<Channel *>::size_type get_ch_set_size() const;
		std::set<Client *>::size_type get_cli_set_size() const;

		bool check_pass_flag_cli_exit(Client *cli);
		void password_err_message(Client *cli);
		pollfd* find_fds(int socket_num);
		~Server();
};

#endif
