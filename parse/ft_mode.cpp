#include "parse.hpp"

/*
	Todo
	* 옵션별로 메세지를 쪼개서 보내는것이 아니라 에러옵션이나 상황을 뺀 나머지를 합쳐서 보냄 - 수정예정
*/
void ft_mode_execute(std::vector<std::string> &recv_vector, Channel *ch, Client *cli, std::vector<std::string>::size_type recv_size)
{
	std::vector<std::string>::size_type mode_argv_idx = 2;
	
	char flag = '\0';
	int idx = 0;

	while (recv_vector[2][idx])
	{
		if (flag == '\0')
		{
			if (recv_vector[2][idx] == '+')
				flag = '+';
			else if (recv_vector[2][idx] == '-')
				flag = '-';
			else
				// ft_send
			idx++;
			continue;
		}
		switch (recv_vector[2][idx])
		{
			case 'i':
				if (flag == '+')
					ch->set_mode_invite(true);
				else if (flag == '-')
					ch->set_mode_invite(false);
				break;
			case 't':
				if (flag == '+')
					ch->set_mode_topic(true);
				else if (flag == '-')
					ch->set_mode_topic(false);
				break;
			case 'k':
				if (mode_argv_idx < recv_size)
				{
					std::string message;
					if (flag == '+')
					{
						for (int j = 0; recv_vector[mode_argv_idx][j]; j++)
						{
							if (!isprint(recv_vector[mode_argv_idx][j]))
							{
								ft_send(ERR_INVALIDKEY, ch->get_ch_name() + " :Key is not well-formed", cli, false);
								break;
							}
						}
						message = "MODE " + ch->get_ch_name() + "+k " + recv_vector[mode_argv_idx];
						ch->set_mode_key(true);
						ch->send_to_ch(message, cli);
						ch->set_passwd(recv_vector[mode_argv_idx]);
						mode_argv_idx++;
					}
					else if (flag == '-')
					{
						message = "MODE " + ch->get_ch_name() + "-k " + ch->get_passwd();
						ch->set_mode_key(false);
						ch->send_to_ch(message, cli);
						std::string passwd="";
						ch->set_passwd(passwd);
					}
				}
				break;
			case 'o':
				if (mode_argv_idx < recv_size)
				{
					/*
						1. 채널에 사용자가 없으면 에러
						2. 채널에 사용자가 있다면 명령어를 실행한 결과를 채널에 알린다
							ex) MODE <channel> +o <권한 받은 유저>
					*/
					Client *user = ch->find_cli_in_ch_by_str(recv_vector[mode_argv_idx]);
					std::string message;
					if (user == NULL)
					{
						ft_send(ERR_USERNOTINCHANNEL,  cli->get_nick_name() + " " + recv_vector[mode_argv_idx] + " " + ch->get_ch_name() + " :They aren't on that channel", cli, true);
						break ;
					}
					if (flag == '+')
					{
						ch->insert_cli_gm(user);
						message = "MODE " + ch->get_ch_name() + " +o " + user->get_nick_name();
					}
					else if (flag == '-')
					{
						ch->delete_cli_gm(user);
						message = "MODE " + ch->get_ch_name() + " -o " + user->get_nick_name();
					}
					else
						break;
					ch->send_to_ch(message, cli);
					mode_argv_idx++;
				}
				break;
			case 'l':
				if (mode_argv_idx < recv_size)
				{
					std::string message;
					if (flag == '+')
					{
						if (!string_isalnum(recv_vector[mode_argv_idx]))
							break;
					}
					else if (flag == '-')
					{
						ch->set_mode_key(false);
					}
					else
						break;
				}
				break;
			default:
				//ft_send
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
	std::vector<std::string>::size_type recv_size = recv_vector.size();
	//채널이 설정되지 않은 경우
	if (recv_size < 2 || return_string_type(recv_vector[1]) != CHANNEL)
	{
		ft_send(ERR_NOSUCHCHANNEL, ":No such channel", cli, true);
		return ;
	}
	std::string ch_name = recv_vector[1];
	Channel *ch = serv.find_ch_with_ch_name(ch_name);
	// 조회용, ex) /mode #channel
	if (recv_size < 3)
		ft_send(RPL_CHANNELMODEIS, cli->get_nick_name() + " " + ch_name + " " + ch->total_mode_string() , cli, false);
	else if (ch->find_cli_in_gm_lst(cli) == false)
		ft_send(RPL_CHANNELMODEIS, cli->get_nick_name() + " " + ch_name + " :You're not channel operator" , cli, false);
	else
		ft_mode_execute(recv_vector, ch, cli, recv_size);
}
