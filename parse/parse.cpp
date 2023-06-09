#include "parse.hpp"

void ft_quit(std::vector<std::string> &recv_vector, Client *cli, Server &serv)
{
	if (recv_vector.size() == 1)

	(void)recv_vector;
	(void)cli;
	(void)serv;
}

void ft_cap(std::vector<std::string> &recv_vector, Client *cli, Server &serv)
{
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
		if (cli->pass_flag == true && cli->nick_flag == true)
		{
			recv_vector = parse_split[i];
			switch(serv.get_cmd(recv_vector[0].c_str()))
			{
				case PING:
					ft_ping(recv_vector, cli, serv);
					break;	
				case PASS:
					ft_send(ERR_ALREADYREGISTERED, ":You may not reregister", cli);
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
				case PART:
					ft_part(recv_vector, cli, serv);
					break;
				default:
					ft_send(ERR_NOTREGISTERED, ":You have not registered", cli);
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
						return ;
					break;
				default:
					ft_send(ERR_NOTREGISTERED, ":You have not registered", cli);
					break;
			}
		}
	}
	parse_split.clear();
}