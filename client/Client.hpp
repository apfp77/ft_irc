#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>

class Client {
	private:
		/*
			권한
		*/
		bool type;
		/*
			nick_name 중복체크
		*/
		std::string nick_name;
		std::string user_name;
		int socket_fd;

	public:
		Client();
		~Client();
};

#endif
