#include "parse.hpp"

void ft_join(std::vector<std::string> &recv_vector, Client *cli, Server &serv)
{
	if (recv_vector.size() < 1)
	{
		ft_send(ERR_NOSUCHCHANNEL, cli->get_nick_name() + "!ft_irc@ft_irc" + " :No such channel" + recv_vector[1] , cli);
		return ;
	}
	else if (recv_vector.size() < 2 || recv_vector[2][0] == ':')
	{
		ft_send(ERR_NEEDMOREPARAMS, cli->get_nick_name() + "!ft_irc@ft_irc" + " :Not enough parameters", cli);
		return ;
	}
	std::vector <std::string> ch_split = ft_split(recv_vector[1], ",");
	std::vector <std::string> pw_split;
	std::vector <std::vector<std::string> >::size_type ch_size = ch_split.size();

	if (recv_vector.size() == 2)
	{
		for (std::vector <std::vector<std::string> >::size_type i = 0; i < ch_size; i++)
			pw_split.push_back("");
	}
	else
		pw_split = ft_split(recv_vector[2], ",");
	for (std::vector <std::vector<std::string> >::size_type i = 0; i < ch_size; i++)
	{
		Channel *join_ch = serv.find_ch_with_ch_name(ch_split[i]);
		if (join_ch == NULL)
		{
			join_ch = new Channel(ch_split[i], cli);
			join_ch->insert_cli(cli);
			join_ch->insert_cli_gm(cli);
			join_ch->set_passwd(pw_split[i]);
			join_ch->set_topic("");
			serv.insert_ch(join_ch);
			ret_join_after_names_message(join_ch, cli);
		}
		else
		{
			if (join_ch->get_passwd().size() > 1 && join_ch->get_passwd() != pw_split[i] && !join_ch->check_cli_in_invite_cli_set(cli))
				ft_send(ERR_BADCHANNELKEY, cli->get_nick_name() + "!ft_irc@ft_irc"+ " " + join_ch->get_ch_name() + " :Cannot join channel (+k)", cli);
			else if (join_ch->get_mode_limit() && join_ch->get_cli_limit() <= (static_cast<int>(join_ch->get_cli_lst_size())))
				ft_send(ERR_CHANNELISFULL, cli->get_nick_name() + "!ft_irc@ft_irc"+ " " + join_ch->get_ch_name() + " :Cannot join channel (+l)", cli);
			//invite이 활성화되어 있고, 초대가 되어있는지 확인함
			else if (join_ch->get_mode_invite() && !join_ch->check_cli_in_invite_cli_set(cli))
				ft_send(ERR_INVITEONLYCHAN, cli->get_nick_name() + "!ft_irc@ft_irc"+ " " + join_ch->get_ch_name() + " :Cannot join channel (+i)", cli);
			else
			{
				if (join_ch->get_topic() != "")
					ft_send(RPL_TOPIC, cli->get_nick_name() + "!ft_irc@ft_irc" + " " + join_ch->get_ch_name() + " :" + join_ch->get_topic(), cli);
				join_ch->insert_cli(cli);
				ret_join_after_names_message(join_ch, cli);
			}
		}
	}
}