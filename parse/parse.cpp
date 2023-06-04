#include "parse.hpp"
#include "Numerics.hpp"
#include "ft_utils.hpp"

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
void ft_privmsg(std::vector<std::string> &recv_vector, Client *cli, Server &serv)
{
	/*
		ERR_NOSUCHNICK (401) : 닉네임이 x
		ERR_CANNOTSENDTOCHAN (404) : 사용자의 메세지를 채널에 전달하지 못할경우 사용됨
		ERR_TOOMANYTARGETS (407) : 아규먼트가 많은 경우 발생 (irc protocol에 privmsg에 써있지만 정의되어 있지않음)
		ERR_NORECIPIENT (411) : 수신자가 없는경우 (채널, 클라이언트가 안써있는 경우)
		ERR_NOTEXTTOSEND (412) : 보낼 텍스트가 없는 경우
		ERR_NOTOPLEVEL (413) : 서버차원에서 최상위
		ERR_WILDTOPLEVEL (414) : 와일드카드 관련 에러
		RPL_AWAY (301) : 사용자가 자리비움인경우
	*/
	/*
		irssi) PRIVMSG #channel :test
		limechat) PRIVMSG #channel test
		limechat은 문자열을 평문으로 보내고irssi는 :를 붙인다
		첫번째만 확인하고 나머진 문자열로 취급하나 2번째 인덱스의 첫 문자가 :인경우를 제외시킨다
	*/

	std::vector<std::string>::size_type recv_size = recv_vector.size();
	if (recv_size < 3)
	{
		ft_send(ERR_NOTEXTTOSEND, ":No text to send", cli, true);
	}
	else if (recv_vector[1][0] == CHANNEL)
	{
		std::string ch_name = recv_vector[1];
		Channel *privmsg_ch = serv.find_ch_with_ch_name(ch_name);
		
		//채널 이름이 없거나, 채널에 소속되지 않은 경우
		if (privmsg_ch == NULL 
			|| privmsg_ch->find_cli_in_ch(cli) == NULL)
		{
			ft_send(ERR_CANNOTSENDTOCHAN, ":Cannot send to channel", cli, true);
			return ;
		}
		
		std::string s;
		for (std::vector<std::string>::size_type i = 2; i < recv_size; i++)
			s += recv_vector[i];
		privmsg_ch->send_to_ch(s);
	}
	else if (recv_vector[1][0] == USER)
	{
		std::string nick_name = recv_vector[1].substr(1);
		Client *privmsg_cli = serv.find_cli_with_nick_name(nick_name);
		if (privmsg_cli == NULL)
		{
			ft_send(ERR_NOSUCHNICK, ":No such nick/channel", cli, true);
			return ;
		}
		//	문자열 합쳐서 해당 클라이언트에 보내기
		std::string s;
		for (std::vector<std::string>::size_type i = 2; i < recv_size; i++)
			s += recv_vector[i];
		ft_send("", s, privmsg_cli, false);
	}
	else
	{
		// std::cout << recv_vector[1] << '\n';
		ft_send(ERR_NORECIPIENT, ":No recipient given", cli, true);
	}
}

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
	
	if (recv_vector.size() < 1 || !(serv.find_ch_with_ch_name(recv_vector[1])))
	{
		ft_send(ERR_NOSUCHCHANNEL, ":No such channel" + recv_vector[1] , cli, true);
		return ;
	}
	else if (recv_vector.size() < 2 || !(serv.find_ch_with_ch_name(recv_vector[1])))
	{
		ft_send(ERR_NEEDMOREPARAMS, "<command> :Not enough parameters", cli, true);
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

void ft_mode(std::vector<std::string> &recv_vector, Client *cli, Server &serv)
{
	/*
		요구사항에서 MODE는 채널 모드 변경만을 요구하므로 사용자는 해석할 수 없습니다
	*/
	/*
		1. 채널이 아니거나 
		2. 채널이 없거나 
		3. 클라이언트에 권한이 없으면 에러
	*/
	std::vector<std::string>::size_type recv_size = recv_vector.size();
	if (recv_size < 2 || return_string_type(recv_vector[1]) != CHANNEL || recv_vector[1][0] == '\0')
	{
		ft_send(ERR_NOSUCHCHANNEL, ":No such channel", cli, true);
		return ;
	}
	std::string ch_name = recv_vector[1];
	// Channel *ch = serv.find_ch_with_ch_name(ch_name);
	/*
		크기가 3이하거나 허용하지 않는 문자가 있을경우(+, -, 
	*/
	// if (recv_size < 3 || ch->is_allow_option(recv_vector[2]))
	{
		ft_send(RPL_CREATIONTIME, cli->get_nick_name() + " #" + ch_name + " +" , cli, false);
		//RPL_CREATIONTIME (329): 채널생성 시간은 선택
		return ;
	}
	
	ft_send(ERR_NOTREGISTERED, ":You have not registered", cli, true);
	(void)recv_vector;
	(void)cli;
	(void)serv;
}

void ft_kick(std::vector<std::string> &recv_vector, Client *cli, Server &serv)
{
	(void)recv_vector;
	(void)cli;
	(void)serv;
}

void ft_quit(std::vector<std::string> &recv_vector, Client *cli, Server &serv)
{
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