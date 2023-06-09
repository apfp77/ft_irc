#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <poll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>

class Client {
	private:
		/*
			nick_name 중복체크
		*/
		std::string nick_name;
		std::string user_name;
		
		int cli_sock;
		struct sockaddr_in cli_sockaddr;
		socklen_t cli_sockaddr_l;
		std::string sub_memory;
		int fds_index;

	public:
	/*
		cli_sockaddr, cli_sockaddr_l은 main.cpp에 테스트용으로 있음
	*/
		struct sockaddr_in &get_cil_addr();
		socklen_t &get_cli_size();
		void setting_socket();
		int append_submemory(char *str);
		std::string division_cmd(int idx);
		bool pass_flag;
		bool nick_flag;
		int get_socket() const;
		void set_socket(int cli_sock);
		void set_nick_name(std::string &nick_name);
		std::string get_nick_name() const;
		void set_fds_index(int fds_index);
		int get_fds_index() const;
		Client();
		~Client();
};

#endif
