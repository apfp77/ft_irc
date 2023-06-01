#include "parse.hpp"
#include "Numerics.hpp"

std::vector <std::string> split(std::string &str, std::string delimiter)
{
	std::vector <std::string>ret;
	std::string::size_type i = 0;
	std::string::size_type tmp_i = 0;
	std::string::size_type str_size = str.length();

	while (tmp_i != str_size)
	{
		i = tmp_i;
		i = str.find(delimiter, i);
		if (i == std::string::npos)
			break;
		ret.push_back(str.substr(tmp_i ,i - tmp_i));
		tmp_i = i + delimiter.length();
	}
	if (tmp_i != str_size)
		ret.push_back(str.substr(tmp_i ,str_size - tmp_i));
	return (ret);
}

void ft_send(std::string code, std::string s, Client *cli)
{
	std::string ret = code + " " + s + "\r\n";
	int check = send(cli->get_socket(), ret.c_str(), ret.length(), 0);
	if (check == -1)
	{ 
		std::cerr << "Failed to send data" << std::endl;
		/*
			클라이언트 클래스 해제
		*/
	}
}

void ft_pass(std::vector<std::string> &recv_vector, Client *cli, Server &serv)
{
	if (cli->pass_flag == true)
		ft_send(ERR_ALREADYREGISTERED, ":You may not reregister", cli);
	if (recv_vector.size() == 2 && !recv_vector[1].compare(serv.get_passwd()))
	{
		cli->pass_flag = true;
		return ;
	}
	/*
		오류 보낸 후  연결종료
	*/
	if (recv_vector.size() != 2)
		ft_send(ERR_NEEDMOREPARAMS, ":Not enough parameters", cli);
	else
		ft_send(ERR_PASSWDMISMATCH, ":Password incorrect", cli);
	/*
		연결 끊는 작업필요, close(소켓) delete cli, pollfd 처리까지
	*/
}

void ft_ping(std::vector<std::string> &recv_vector, Client *cli, Server &serv)
{
	if (recv_vector.size() != 2)
	{
		ft_send(ERR_NEEDMOREPARAMS, ":Not enough parameters", cli);
		return ;
	}
	std::string ret = "PONG " + recv_vector[1] + "\r\n";
	if (send(cli->get_socket(), ret.c_str(), ret.length(), 0) == -1)
	{
		/*
			연결 끊는 작업필요, close(소켓) delete cli, pollfd 처리까지
		*/
	}
	(void)serv;
}

bool string_isalnum(std::string &s)
{
	if (s.length() == 0)
		return (false);
	for (int i = 0; s[i]; i++)
	{
		if (!std::isalnum(s[i]))
			return (false);
	}
	return (true);
}

void ft_nick(std::vector<std::string> &recv_vector, Client *cli, Server &serv)
{
	/*
		매개변수가 없는경우
		ERR_NO`GIVEN
	*/
	/*
		nick에 허용하지 않는 문자가 포함될 경우
		ERR_ERRONEUSNICKNAME
	*/
	/*
		이미 동일한 nick이 존재할경우
		ERR_NICKNAMEINUSE
	*/
	/*
		다른 서버에 nick이 존재할경우 (우리 서버는 하나이므로 처리하지 않음)
		ERR_NICKCOLLISION
	*/
	if (recv_vector.size() != 2)
		ft_send(ERR_NONICKNAMEGIVEN, ":No nickname given", cli);
	else if (!string_isalnum(recv_vector[1]))
		ft_send(ERR_ERRONEUSNICKNAME, ":Erroneus nickname", cli);
	else if (serv.find_cli_with_nick_name(recv_vector[1]))
		ft_send(ERR_NICKNAMEINUSE, ":Nickname is already in use", cli);
	else
	{
		if (cli->get_nick_name() == "")
			ft_send(RPL_WELCOME, ":Welcome to the ft_irc Network " + recv_vector[1] , cli);
		cli->set_nick_name(recv_vector[1]);
	}
}

void ft_name(std::vector<std::string> &recv_vector, Client *cli, Server &serv)
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
	if (recv_vector.size() < 3)
	{
		ft_send(ERR_NOTEXTTOSEND, ":No text to send", cli);
	}
	else if (recv_vector[1][0] == '#')
	{
		Channel *ch = serv.find_ch_with_nick_name(recv_vector[1]);
		if (ch == NULL || ch->find_cli_in_ch(recv_vector[1]) == NULL)
		{
			ft_send(ERR_CANNOTSENDTOCHAN, ":Cannot send to channel", cli);
			return ;
		}
		/*
			채널에 가입한 클라이언트에 전송
		*/
	}
	else if (recv_vector[1][0] == '@')
	{
		Client *cli = serv.find_cli_with_nick_name(recv_vector[1]);
		if (cli == NULL)
		{
			ft_send(ERR_NOSUCHNICK, ":No such nick/channel", cli);
			return ;
		}
		/*
			문자열 합쳐서 해당 클라이언트에 보내기
		*/
	}
	else
	{
		ft_send(ERR_NORECIPIENT, ":No recipient given", cli);
	}
	(void)recv_vector;
	(void)serv;
}

void ft_topic(std::vector<std::string> &recv_vector, Client *cli, Server &serv)
{
	(void)recv_vector;
	(void)cli;
	(void)serv;
}

void ft_join(std::vector<std::string> &recv_vector, Client *cli, Server &serv)
{
	if (recv_vector.size() < 1 || !(serv.find_ch_with_nick_name(recv_vector[1])))
	{
		ft_send(ERR_NOSUCHCHANNEL, ":No such channel" + recv_vector[1] , cli);
		return ;
	}
	(void)recv_vector;
	(void)cli;
	(void)serv;
}

void ft_mode(std::vector<std::string> &recv_vector, Client *cli, Server &serv)
{
	ft_send(ERR_NOTREGISTERED, ":You have not registered", cli);
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

/*
	irssi에서 초기 접근 순서
	1. CAP LS 302
	2. PASS
	3. NICK
	4. USER
*/
void parse(std::string recv, Client *cli, Server &serv)
{
	std::vector <std::vector<std::string> > parse_split;
	std::vector <std::string> recv_vector;
	recv_vector = split(recv, "\r\n");
	for (std::vector<std::string>::size_type i = 0; i < recv_vector.size(); i++)
		parse_split.push_back(split(recv_vector[i], " "));
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
			case NAME:
				ft_name(recv_vector, cli, serv);
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
			default:
				ft_send(ERR_NOTREGISTERED, ":You have not registered", cli);
				break;
		}
	}
}