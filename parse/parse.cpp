#include "parse.hpp"

void ft_pass(std::vector<std::string> &recv_vector, Client *cli, Server &serv)
{
	if (cli->pass_flag == true)
		ft_send(ERR_ALREADYREGISTERED, ":You may not reregister", cli, true);
	if (recv_vector.size() == 2 && !recv_vector[1].compare(serv.get_passwd()))
	{
		cli->pass_flag = true;
		return ;
	}
	/*
		Todo
		패스워드가 일치하지 않을 경우 연결 끊는 작업 필요(close, client delete, pollfd 등)
	*/
	if (recv_vector.size() != 2)
		ft_send(ERR_NEEDMOREPARAMS, ":Not enough parameters", cli, true);
	else
		ft_send(ERR_PASSWDMISMATCH, ":Password incorrect", cli, true);
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

void ft_nick(std::vector<std::string> &recv_vector, Client *cli, Server &serv)
{
	/*
		1. ERR_NOICKGIVEN: 매개변수가 없는경우
		2. ERR_ERRONEUSNICKNAME: nick에 허용하지 않는 문자가 포함될 경우
		3. ERR_NICKNAMEINUSE: 이미 동일한 nick이 존재할경우
		4. ERR_NICKCOLLISION: 다른 서버에 nick이 존재할경우 (우리 서버는 하나이므로 처리하지 않음)
	*/
	if (recv_vector.size() != 2)
		ft_send(ERR_NONICKNAMEGIVEN, ":No nickname given", cli, true);
	else if (!string_isalnum(recv_vector[1]))
		ft_send(ERR_ERRONEUSNICKNAME, ":Erroneus nickname", cli, true);
	else if (serv.find_cli_with_nick_name(recv_vector[1]))
		ft_send(ERR_NICKNAMEINUSE, ":Nickname is already in use", cli, true);
	else
	{
		// 	ft_send(RPL_WELCOME, ":Welcome to the ft_irc Network " + recv_vector[1] , cli, false);
		//현재 클라이언트가 접속한 ip를 띄울것
		if (cli->get_nick_name() == "")
		{
			ft_send(RPL_WELCOME, recv_vector[1] + " :Welcome to the ft_irc Network " + recv_vector[1] , cli, false);
			ft_send(RPL_YOURHOST, recv_vector[1] + " :Your nickname is " + recv_vector[1] + ", running version", cli, false);
			serv.insert_cli(cli);
		}
		cli->set_nick_name(recv_vector[1]);
	}
}

void ft_names(std::vector<std::string> &recv_vector, Client *cli, Server &serv)
{
	(void)recv_vector;
	(void)cli;
	(void)serv;
}

/*
	PASS를 통과하지 못하면 서버와의 연결을 끊어버린다
*/


void ft_topic(std::vector<std::string> &recv_vector, Client *cli, Server &serv)
{
	(void)recv_vector;
	(void)cli;
	(void)serv;
}

void ft_join(std::vector<std::string> &recv_vector, Client *cli, Server &serv)
{
	// ERR_NEEDMOREPARAMS (461)		-
	// ERR_NOSUCHCHANNEL (403)
	// ERR_TOOMANYCHANNELS (405)
	// ERR_BADCHANNELKEY (475)		-
	// ERR_BANNEDFROMCHAN (474)
	// ERR_CHANNELISFULL (471)		-
	// ERR_INVITEONLYCHAN (473)		-
	// ERR_BADCHANMASK (476)
	// RPL_TOPIC (332)
	// RPL_TOPICWHOTIME (333)
	// RPL_NAMREPLY (353)
	// RPL_ENDOFNAMES (366)
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
	else if (recv_vector.size() < 2)
	{
		ft_send(ERR_NEEDMOREPARAMS, recv_vector[1] + " :Not enough parameters", cli, true);
		return ;
	}
	Channel *join_ch = serv.find_ch_with_ch_name(recv_vector[1]);
	if (join_ch == NULL)
	{
		join_ch = new Channel(recv_vector[1], cli);
		join_ch->insert_cli(cli);
		join_ch->insert_cli_gm(cli);
		join_ch->set_passwd(recv_vector[2]);
		serv.insert_ch(join_ch);
	}
	else
	{
		if (join_ch->get_passwd() != recv_vector[2])
		{
			ft_send(ERR_BADCHANNELKEY, join_ch->get_ch_name() + " :Cannot join channel (+k)", cli, true);
			return ;
		}
		else if (join_ch->get_cli_limit() > 0 && join_ch->get_cli_limit() < (static_cast<int>(join_ch->get_cli_lst_size())))
		{
			ft_send(ERR_CHANNELISFULL, join_ch->get_ch_name() + " :Cannot join channel (+l)", cli, true);
			return ;
		}
		else if (join_ch->get_mode_invite())
		{
			ft_send(ERR_INVITEONLYCHAN, join_ch->get_ch_name() + " :Cannot join channel (+i)", cli, true);
			return ;
		}
		join_ch->insert_cli(cli);
	}
	(void)recv_vector;
	(void)cli;
	(void)serv;
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
		recv_vector = parse_split[i];
		switch(serv.get_cmd(recv_vector[0].c_str()))
		{
			case PING:
				ft_ping(recv_vector, cli, serv);
				break;	
			case PASS:
				ft_pass(recv_vector, cli, serv);
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
				ft_quit(recv_vector, cli, serv);
				break;
			case CAP:
				ft_cap(recv_vector, cli, serv);
				break;
			default:
				ft_send(ERR_NOTREGISTERED, ":You have not registered", cli, true);
				break;
		}
	}
}