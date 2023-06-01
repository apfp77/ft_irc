#ifndef MODE_HPP
#define MODE_HPP

#include <iostream>
#include <set>
#include "Client.hpp"

class Mode {
	private:
		/*
			o옵션은 Channel.hpp에서 gm_lst에서 뺴는걸로
		*/
		bool mode_invite;
		bool mode_topic;
		bool mode_key;
		bool mode_limit;
		std::string passwd;
		/*
			초대를 받은 클라이언트 리스트, 운영자는 기본적으로 포함해야함
		*/
		std::set<std::string> invite_cli_set;
		// std::map<std::string, Client *> invite_cli_map;

		/*
			클라이언트 제한 수
		*/
		int cli_limit;

	public:
		Mode();
		~Mode();
};

#endif
