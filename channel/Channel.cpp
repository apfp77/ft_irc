#include "Channel.hpp"

void topic_update()
{

}

void Channel::topic(int a)
{
	std::cout << a << '\n';
}

std::map<std::string, void *> Channel::cmds;
