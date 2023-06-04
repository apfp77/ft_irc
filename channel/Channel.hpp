#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include "Mode.hpp"
#include "Client.hpp"
#include <set>

/*
	채널 동작방식
	1. 채널을 생성하는 사람은 관리자가 된다
	2. 서버와의 연결을 종료했는데 채널의 관리자라면 임의로 관리자가 설정된다
		 - 단, 종료한 클라이언트가 유일한 관리자여야함
	3. 채널에 가입한 사람이 없을경우 채널은 삭제된다
*/
/*
	채널 존재유무확인함수
*/
/*
	없다면 에러전송
*/
class Channel : public Mode {
	private:
		std::string ch_name;
		std::set<Client *> gm_lst;
		std::string passwd;
		/*
			Limechat 에서는 지우는 명령어가 없고 irssi -delete옵션을 넣으면 지워짐
		*/
		std::string topic_message;
		std::set<Client *> cli_lst;
		// Mode mode;

	public:
		Channel(std::string &ch_name, Client *cli);
		~Channel();
		std::string get_ch_name() const;
		
		void set_passwd(std::string &passwd);
		std::string get_passwd() const;

		std::set<Client *>::size_type get_cli_lst_size() const;

		Client* find_cli_in_ch(Client *cli);
		void send_to_ch(std::string &message, Client *cli);

		void insert_cli(Client *cli);
		void insert_cli_gm(Client *cli);

		void delete_cli(Client *cli);
		void delete_cli_gm(Client *cli);

		std::string get_topic() const;
		void set_topic(Client *cli, std::string &new_message);

};

#endif
