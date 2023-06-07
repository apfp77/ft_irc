#include "parse.hpp"

bool ft_pass(std::vector<std::string> &recv_vector, Client *cli, Server &serv)
{
	if (recv_vector.size() == 2 && !recv_vector[1].compare(serv.get_passwd()))
	{
		cli->pass_flag = true;
		return (false);
	}
	if (recv_vector.size() != 2)
	{
		ft_send(ERR_NEEDMOREPARAMS, ":Not enough parameters", cli, true);
		serv.check_pass_flag_cli_exit(cli);
	}
	else
	{
		serv.password_err_message(cli);
		serv.check_pass_flag_cli_exit(cli);
	}
	return (true);
}

void ft_ping(std::vector<std::string> &recv_vector, Client *cli, Server &serv)
{
	if (recv_vector.size() != 2)
	{
		ft_send(ERR_NEEDMOREPARAMS, ":Not enough parameters", cli, true);
		return ;
	}
	std::string ret = "PONG " + recv_vector[1] + "\r\n";
	ft_send("", ret, cli, false);
	(void)serv;
}


/*
	PASS를 통과하지 못하면 서버와의 연결을 끊어버린다
*/


void ft_topic(std::vector<std::string> &recv_vector, Client *cli, Server &serv)
{
	if (recv_vector.size() < 2)
	{
		ft_send(ERR_NEEDMOREPARAMS, recv_vector[1] + " :Not enough parameters", cli, true);
		return ;
	}
	Channel *topic_ch = serv.find_ch_with_ch_name(recv_vector[1]);
	if (topic_ch == NULL)
	{
		ft_send(ERR_NOSUCHCHANNEL, "IRSSI " + recv_vector[1] + " :No such channel", cli, true);
		return ;
	}
	else if (topic_ch->find_cli_in_ch(cli) == NULL)
	{
		ft_send(ERR_NOTONCHANNEL, "IRSSI " + topic_ch->get_ch_name() + " :You're not on that channel", cli, true);
		return ;
	}
	if (recv_vector.size() == 2)
	{
		if (topic_ch->get_topic() == "")
			ft_send(RPL_NOTOPIC, "IRSSI " + topic_ch->get_ch_name() + " :No topic is set", cli, false);
		else
			ft_send(RPL_TOPIC, "IRSSI " + topic_ch->get_ch_name() + " :" + topic_ch->get_topic(), cli, false);
	}
	else if (recv_vector.size() > 2)
	{
		if (topic_ch->get_mode_topic() && !topic_ch->find_cli_in_gm_lst(cli))
		{
			// ft_send(ERR_CHANOPRIVSNEEDED, "IRSSI " + topic_ch->get_ch_name() + " :You're not channel operator", cli, true);
			ft_send(ERR_CHANOPRIVSNEEDED, cli->get_nick_name() + " " + topic_ch->get_ch_name() + " :You're not channel operator", cli, true);
			return ;
		}
		topic_ch->set_topic(cli, recv_vector[2]);
		std::string s = ":" + cli->get_nick_name() + " TOPIC " + topic_ch->get_ch_name() + " :" + topic_ch->get_topic();
		ft_send("", s, cli, false);
		topic_ch->send_to_ch(s, cli);
	}
}

void ft_join(std::vector<std::string> &recv_vector, Client *cli, Server &serv)
{
	/*
		Todo
		채널 생성시 서버에도 넣어주셔야해요
		서버에 채널 추가: insert_ch
		채널에 클라이언트 추가: insert_cli
		채널에 운영자 추가: insert_cli_gm
			- 운영자는 insert_cli, insert_cli_gm 둘다 넣어주세요
		Mode는 아직 구현안했어요
	*/
	if (recv_vector.size() < 1)
	{
		ft_send(ERR_NOSUCHCHANNEL, ":No such channel" + recv_vector[1] , cli, true);
		return ;
	}
	else if (recv_vector.size() < 2 || recv_vector[2][0] == ':')
	{
		ft_send(ERR_NEEDMOREPARAMS, recv_vector[1] + " :Not enough parameters", cli, true);
		return ;
	}
	std::vector <std::string> ch_split;
	std::vector <std::string> pw_split;
	std::string topic;
	std::vector <std::vector<std::string> >::size_type ch_size;
	std::vector <std::vector<std::string> >::size_type pw_size;
	ch_split = ft_split(recv_vector[1], ",");
	ch_size = ch_split.size();
	if (recv_vector.size() == 2)
	{
		for (std::vector <std::vector<std::string> >::size_type i = 0; i < ch_size; i++)
			pw_split.push_back("");
	}
	else
		pw_split = ft_split(recv_vector[2], ",");
	pw_size = pw_split.size();
	for (std::vector <std::vector<std::string> >::size_type i = 0; i < ch_size; i++)
	{
		Channel *join_ch = serv.find_ch_with_ch_name(ch_split[i]);
		if (join_ch == NULL)
		{
			join_ch = new Channel(ch_split[i], cli);
			join_ch->insert_cli(cli);
			join_ch->insert_cli_gm(cli);
			join_ch->set_passwd(pw_split[i]);
			topic = "";
			join_ch->set_topic(cli, topic);
			serv.insert_ch(join_ch);
			ret_join_after_names_message(join_ch, cli);
		}
		else
		{
			if (join_ch->get_passwd().size() > 1 && join_ch->get_passwd() != pw_split[i])
				ft_send(ERR_BADCHANNELKEY, "IRSSI " + join_ch->get_ch_name() + " :Cannot join channel (+k)", cli, true);
			else if (join_ch->get_cli_limit() > 0 && join_ch->get_cli_limit() < (static_cast<int>(join_ch->get_cli_lst_size())))
				ft_send(ERR_CHANNELISFULL, "IRSSI " + join_ch->get_ch_name() + " :Cannot join channel (+l)", cli, true);
			else if (join_ch->get_mode_invite())
				ft_send(ERR_INVITEONLYCHAN, "IRSSI " + join_ch->get_ch_name() + " :Cannot join channel (+i)", cli, true);
			else
			{
				if (join_ch->get_topic() != "")
					ft_send(RPL_TOPIC, "IRSSI " + join_ch->get_ch_name() + " :" + join_ch->get_topic(), cli, false);
				join_ch->insert_cli(cli);
				ret_join_after_names_message(join_ch, cli);
			}
		}
	}
}

void ft_quit(std::vector<std::string> &recv_vector, Client *cli, Server &serv)
{
	if (recv_vector.size() == 1)

	(void)recv_vector;
	(void)cli;
	(void)serv;
}

void ft_cap(std::vector<std::string> &recv_vector, Client *cli, Server &serv)
{
	ft_send(ERR_NOTREGISTERED, ":You have not registered", cli, true);
	(void)recv_vector;
	(void)cli;
	(void)serv;
}

/*
	irssi에서 초기 접근 순서
	1. CAP LS 302
	2. PASS(인증)
	3. NICK
	4. USER
*/
void parse(std::string recv, Client *cli, Server &serv)
{
	/*
		Todo
		인증 전에 PING과 QUIT, 등록되지 않은 명령어를 제외한 명령어가 들어오면
		클라이언트와의 연결을 끊는다
	*/
	std::vector <std::vector<std::string> > parse_split;
	std::vector <std::string> recv_vector;
	recv_vector = ft_split(recv, "\r\n");
	for (std::vector<std::string>::size_type i = 0; i < recv_vector.size(); i++)
		parse_split.push_back(ft_split(recv_vector[i], " "));
	recv_vector.clear();
	for (std::vector <std::vector<std::string> >::size_type i = 0; i < parse_split.size(); i++)
	{
		if (cli->pass_flag == true)
		{
			recv_vector = parse_split[i];
			switch(serv.get_cmd(recv_vector[0].c_str()))
			{
				case PING:
					ft_ping(recv_vector, cli, serv);
					break;	
				case PASS:
					ft_send(ERR_ALREADYREGISTERED, ":You may not reregister", cli, true);
					break;
				case NICK:
					ft_nick(recv_vector, cli, serv);
					break;
				case NAMES:
					ft_names(recv_vector, cli, serv);
					break;
				case PRIVMSG:
					ft_privmsg(recv_vector, cli, serv);
					break;
				case TOPIC:
					ft_topic(recv_vector, cli, serv);
					break;
				case JOIN:
					ft_join(recv_vector, cli, serv);
					break;
				case MODE:
					ft_mode(recv_vector, cli, serv);
					break;
				case KICK:
					ft_kick(recv_vector, cli, serv);
					break;
				case QUIT:
					ft_quit(recv_vector, cli, serv);
					break;
				case INVITE:
					ft_invite(recv_vector, cli, serv);
					break;
				case CAP:
					ft_cap(recv_vector, cli, serv);
					break;
				default:
					ft_send(ERR_NOTREGISTERED, ":You have not registered", cli, true);
					break;
			}
		}
		else
		{
			recv_vector = parse_split[i];
			switch(serv.get_cmd(recv_vector[0].c_str()))
			{
				case PASS:
					if (ft_pass(recv_vector, cli, serv))
						return ;
					break;
				case CAP:
					ft_cap(recv_vector, cli, serv);
					break;
				case NICK:
					if (ft_connect_nick(recv_vector, cli, serv))
					{
						cli->pass_flag = false;
						serv.check_pass_flag_cli_exit(cli);
						return ;
					}
					break;
				default:
					ft_send(ERR_NOTREGISTERED, ":You have not registered", cli, true);
					break;
			}
		}
	}
	parse_split.clear();
}