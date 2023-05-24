#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include "Client.hpp"
#include <vector>
#include <map>


class Channel {
	private:
		std::string channel_name;
		std::vector<Client *> channel_lst;
		Client *gm;
		/*
			Limechat 에서는 지우는 명령어가 없고 irssi -delete옵션을 넣으면 지워짐
		*/
		std::string topic_v;

	public:
		static std::map<std::string, void *> cmds;
		void topic(int a);
		Channel();
		~Channel();
};


#endif
