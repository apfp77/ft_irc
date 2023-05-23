#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>

class Client {
	private:
		bool type;
		/*
			nick_name = duplicate check
		*/
		std::string nick_name;
		std::string user_name;
		int socket_fd;

	public:
		Client();
		~Client();
};

#endif
