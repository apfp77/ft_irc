#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include "Mode.hpp"
// #include "Client.hpp"
#include <set>



/*
	채널 존재유무확인함수
*/
/*
	없다면 에러전송
*/
class Channel {
	private:
		std::string ch_name;
		std::set<Client *> cli_lst;
		std::set<Client *> gm_lst;
		std::string passwd;
		/*
			Limechat 에서는 지우는 명령어가 없고 irssi -delete옵션을 넣으면 지워짐
		*/
		std::string topic_message;
		Mode mode;

	public:
		std::string get_topic() const;
		std::string get_ch_name() const;
		void set_topic(Client *cli, std::string &new_message);

		Channel(std::string);
		~Channel();
};

#endif
