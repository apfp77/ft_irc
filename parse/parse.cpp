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
	close(cli->get_socket());
	delete cli;
	(void)serv;
}

void ft_ping(std::vector<std::string> &recv_vector, Client *cli, Server &serv)
{
	if (recv_vector.size() != 2)
	{
		ft_send(ERR_NEEDMOREPARAMS, ":Not enough parameters", cli);
		return ;
	}
	std::string ret = "PONG " + recv_vector[1] + "\r\n";
	send(cli->get_socket(), ret.c_str(), ret.length(), 0);
	(void)cli;
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
		ERR_NONICKNAMEGIVEN
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

void ft_privmsg(std::vector<std::string> &recv_vector, Client *cli, Server &serv)
{
	(void)recv_vector;
	(void)cli;
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
	if (recv_vector.size() < 1 || !(serv.get_channel(recv_vector[1])))
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