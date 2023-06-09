#include "parse.hpp"

void ft_topic(std::vector<std::string> &recv_vector, Client *cli, Server &serv)
{
	if (recv_vector.size() < 2)
	{
		ft_send(ERR_NEEDMOREPARAMS, recv_vector[1] + " :Not enough parameters", cli);
		return ;
	}
	Channel *topic_ch = serv.find_ch_with_ch_name(recv_vector[1]);
	if (topic_ch == NULL)
	{
		ft_send(ERR_NOSUCHCHANNEL, recv_vector[1] + " :No such channel", cli);
		return ;
	}
	else if (topic_ch->find_cli_in_ch(cli) == NULL)
	{
		ft_send(ERR_NOTONCHANNEL, cli->get_nick_name() + "!ft_irc@ft_irc" + " :You're not on that channel", cli);
		return ;
	}
	if (recv_vector.size() == 2)
	{
		if (topic_ch->get_topic() == "")
			ft_send(RPL_NOTOPIC, cli->get_nick_name() + "!ft_irc@ft_irc" + " :No topic is set", cli);
		else
			ft_send(RPL_TOPIC, cli->get_nick_name() + "!ft_irc@ft_irc" + " :" + topic_ch->get_topic(), cli);
	}
	else if (recv_vector.size() > 2)
	{
		if (topic_ch->get_mode_topic() && !topic_ch->find_cli_in_gm_lst(cli))
		{
			ft_send(ERR_CHANOPRIVSNEEDED, cli->get_nick_name() + "!ft_irc@ft_irc" + " " + topic_ch->get_ch_name() + " :You're not channel operator", cli);
			return ;
		}
		std::string	topic;
		for (std::vector<std::string>::size_type i = 2; i < recv_vector.size(); i++)
			topic += recv_vector[i] + " ";
		topic_ch->set_topic(topic.substr(1, topic.size() - 2));
		std::string s = ":" + cli->get_nick_name() + "!ft_irc@ft_irc" + " TOPIC " + topic_ch->get_ch_name() + " :" + topic_ch->get_topic();
		ft_send("", s, cli);
		topic_ch->send_to_ch(s, cli);
	}
}
