#include "parse.hpp"

void ft_kick(std::vector<std::string> &recv_vector, Client *cli, Server &serv)
{
	Channel *channel;

	if (recv_vector.size() <= 2)
	{
		ft_send(ERR_NEEDMOREPARAMS, "Not enough parameters", cli, true);
		return ;
	}
	channel = serv.find_ch_with_ch_name(recv_vector[1]);
	if (channel == NULL)
	{
		ft_send(ERR_NOSUCHCHANNEL, "No such channel", cli, true);
		return ;
	}
	if (channel->find_cli_in_ch(cli) == NULL)
	{
		ft_send(ERR_NOTONCHANNEL, "You're not on that channel", cli, true);
		return ;
	}
	if (channel)

	if (channel->find_cli_in_gm_ch(cli) == NULL)
	{
		ft_send(ERR_CHANOPRIVSNEEDED, "You're not channel operator", cli, true);
	}
	if 

}