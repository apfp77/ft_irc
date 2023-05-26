#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include "Client.hpp"
#include <vector>
#include <map>


/*
	채널 존재유무확인함수
*/
/*
	없다면 에러전송
*/



class Channel {
	private:
		std::string channel_name;
		std::vector<Client *> client_lst;
		std::vector<Client *> gm_lst;
		std::string passwd;
		/*
			Limechat 에서는 지우는 명령어가 없고 irssi -delete옵션을 넣으면 지워짐
		*/
		std::string topic_message;
    bool mode_invite = false;
		bool mode_topic = false;
		bool mode_key = false;
		bool mode_limit = false;

	public:
		std::string &get_topic() const;
		void set_topic(Client *client, std::string &new_message);
		Channel(std::string);
		~Channel();
};


#endif
