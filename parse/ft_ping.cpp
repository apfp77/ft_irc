#include "parse.hpp"

void ft_ping(std::vector<std::string> &recv_vector, Client *cli, Server &serv)
{
	if (recv_vector.size() != 2)
	{
		ft_send(ERR_NEEDMOREPARAMS, ":Not enough parameters", cli);
		return ;
	}
	std::string ret = "PONG " + recv_vector[1] + "\r\n";
	ft_send("", ret, cli);
	(void)serv;
}
