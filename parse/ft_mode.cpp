#include "parse.hpp"
#include <sstream>

void ft_mode_execute_invite(Channel *ch, char &flag)
{
	if (flag == '+')
	{
		ch->set_mode_invite(true);
	}
	else if (flag == '-')
	{		
		ch->set_mode_invite(false);
		ch->invite_cli_all_clean();
	}
}
void ft_mode_execute_topic(Channel *ch, char &flag)
{
	if (flag == '+')
		ch->set_mode_topic(true);
	else if (flag == '-')
		ch->set_mode_topic(false);
}

int ft_mode_execute_key(Channel *ch, char &flag, Client *cli, std::string &key, std::string (&messages)[2])
{
	if (flag == '+')
	{
		for (int j = 0; key[j]; j++)
		{
			if (!isprint(key[j]))
			{
				ft_send(ERR_INVALIDKEY, ch->get_ch_name() + " :Key is not well-formed", cli);
				return (1);
			}
		}
		messages[0] += 'k';
		ch->set_mode_key(true);
		ch->set_passwd(key);
		messages[1] += (" " + key);
		return (1);
	}
	else if (flag == '-')
	{
		ch->set_mode_key(false);
		std::string passwd="";
		ch->set_passwd(passwd);
		messages[0] += 'k';
	}
	return (0);
}

/*
	1. 채널에 사용자가 없으면 에러
	2. 채널에 사용자가 있다면 명령어를 실행한 결과를 채널에 알린다
		ex) MODE <channel> +o <권한 받은 유저>
*/
void ft_mode_execute_operator(Channel *ch, char &flag, Client *cli, std::string &nick_name, std::string (&messages)[2])
{
	Client *user = ch->find_cli_in_ch_by_str(nick_name);
	if (user == NULL)
	{
		ft_send(ERR_USERNOTINCHANNEL,  cli->get_nick_name() + " " + nick_name + " " + ch->get_ch_name() + " :They aren't on that channel", cli);
		return ;
	}
	if (flag == '+')
	{
		ch->insert_cli_gm(user);
	}
	else if (flag == '-')
	{
		ch->delete_cli_gm(user);
	}
	messages[0] += 'o';
	messages[1] += (" " + nick_name);
}

int ft_mode_execute_limit(Channel *ch, char &flag, std::string &num, std::string (&messages)[2])
{
	if (flag == '+')
	{
		if (!string_isalnum(num))
		{
			// ERR_NEEDMOREPARAMS (461) or 무시
			return (1);
		}
		std::stringstream ss(num);
		int x;
		ss >> x;
		ch->set_cli_limit(x);
		ch->set_mode_limit(true);
		messages[0] += 'l';
		messages[1] += (" " + num);
		return (1);
	}
	else if (flag == '-')
	{
		messages[0] += 'l';
		ch->set_mode_limit(false);
		ch->set_cli_limit(0);
	}
	return (0);
}

void ft_mode_execute(std::vector<std::string> &recv_vector, Channel *ch, Client *cli, std::string (&messages)[2])
{
	std::vector<std::string>::size_type mode_argv_idx = 3;
	
	char flag = '\0';
	int idx = 0;

	while (recv_vector[2][idx])
	{
		if (flag == '\0')
		{
			flag = recv_vector[2][idx];
			messages[0] += flag;
			if (flag != '+' && flag != '-')
			{
				ft_send(ERR_UNKNOWNMODE, cli->get_nick_name() + " " + recv_vector[2][idx] + " :is unknown mode char to me", cli);
				flag = '\0';
				messages[0].erase(messages[0].end() - 1); 
			}
			idx++;
			continue;
		}
		switch (recv_vector[2][idx])
		{
			case 'i':
				ft_mode_execute_invite(ch, flag);
				messages[0] += 'i';
				break;
			case 't':
				ft_mode_execute_topic(ch, flag);
				messages[0] += 't';
				break;
			case 'k':
				if (mode_argv_idx < recv_vector.size())
					mode_argv_idx += ft_mode_execute_key(ch, flag, cli, recv_vector[mode_argv_idx], messages);
				break;
			case 'o':
				if (mode_argv_idx < recv_vector.size())
				{
					ft_mode_execute_operator(ch, flag, cli, recv_vector[mode_argv_idx], messages);
					mode_argv_idx++;
				}
				break;
			case 'l':
				if (mode_argv_idx < recv_vector.size() || flag == '-')
				{
					mode_argv_idx += ft_mode_execute_limit(ch, flag, recv_vector[mode_argv_idx], messages);
				}
				break;
			default:
				ft_send(ERR_UNKNOWNMODE, cli->get_nick_name() + " " + recv_vector[2][idx] + " :is unknown mode char to me", cli);
				break;
		}
		idx++;
	}
}
/*
	Command: MODE
  Parameters: <target> [<modestring> [<mode arguments>...]]

	* MODE <채널 or 사용자> <옵션> <모드가 변경될 인자(채널 or 사용자)...)>
		ex) MODE #channel +o user1 user2
	* 요구사항에서 MODE는 채널 모드 변경만을 요구하므로 사용자는 해석할 수 없습니다
	* +k, +l 등 값을 필요로 하지만 값이 없을 경우 해당 메세지는 무시한다
*/
void ft_mode(std::vector<std::string> &recv_vector, Client *cli, Server &serv)
{
	std::string messages[2];
	/*
		0: 성공한 옵션 
		1: 성공한 값 
		ex) +kl test 15
	*/
	if (recv_vector.size() < 2 || return_string_type(recv_vector[1]) != CHANNEL)
	{
		ft_send(ERR_NOSUCHCHANNEL, ":No such channel", cli);
		return ;
	}
	std::string ch_name = recv_vector[1];
	Channel *ch = serv.find_ch_with_ch_name(ch_name);
	if (recv_vector.size() < 3)
		ft_send(RPL_CHANNELMODEIS, cli->get_nick_name() + " " + ch_name + " " + ch->total_mode_string() , cli);
	else if (ch->find_cli_in_gm_lst(cli) == false)
		ft_send(ERR_CHANOPRIVSNEEDED, cli->get_nick_name() + " " + ch_name + " :You're not channel operator" , cli);
	else
		ft_mode_execute(recv_vector, ch, cli, messages);
	if (messages[0].length() != 0)
	{
		std::string send_mesasge = ":" + cli->get_nick_name() + "!ft_irc@ft_irc" + " MODE " + ch->get_ch_name() + " ";
		int flag = false;
		if (messages[0].length() > 1)
		{
			flag = true;
			send_mesasge += messages[0];
		}
		if (messages[1].length() != 0)
			send_mesasge += messages[1];
		if (flag)
			ch->all_send_to_ch(send_mesasge);
	}
}
