#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include "Client.hpp"
#include <vector>

class Channel {
	private:
		std::string channel_name;
		std::vector<Client *> channel_lst;

	public:
		Channel();
		~Channel();
};

#endif
