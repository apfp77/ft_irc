#include "parse.hpp"

/*
	1. ERR_NOICKGIVEN: 매개변수가 없는경우
	2. ERR_ERRONEUSNICKNAME: nick에 허용하지 않는 문자가 포함될 경우
	3. ERR_NICKNAMEINUSE: 이미 동일한 nick이 존재할경우
	4. ERR_NICKCOLLISION: 다른 서버에 nick이 존재할경우 (우리 서버는 하나이므로 처리하지 않음)
*/
bool ft_connect_nick(std::vector<std::string> &recv_vector, Client *cli, Server &serv)
{
	if (recv_vector.size() == 2 && string_isalnum(recv_vector[1]) && !serv.find_cli_with_nick_name(recv_vector[1]))
	{
		ft_send(RPL_WELCOME, recv_vector[1] + " :Welcome to the ft_irc Network " + recv_vector[1] , cli, false);
		ft_send(RPL_YOURHOST, recv_vector[1] + " :Your nickname is " + recv_vector[1] + ", running version", cli, false);
		cli->set_nick_name(recv_vector[1]);
		return (false);
	}
	if (recv_vector.size() != 2)
		ft_send(ERR_NONICKNAMEGIVEN, ":No nickname given", cli, true);
	else if (!string_isalnum(recv_vector[1]))
		ft_send(ERR_ERRONEUSNICKNAME, ":Erroneus nickname", cli, true);
	else if (serv.find_cli_with_nick_name(recv_vector[1]))
		ft_send(ERR_NICKNAMEINUSE, ":Nickname is already in use", cli, true);
	return (true);
}

void ft_nick(std::vector<std::string> &recv_vector, Client *cli, Server &serv)
{
	if (recv_vector.size() != 2)
		ft_send(ERR_NONICKNAMEGIVEN, ":No nickname given", cli, true);
	else if (!string_isalnum(recv_vector[1]))
		ft_send(ERR_ERRONEUSNICKNAME, ":Erroneus nickname", cli, true);
	else if (serv.find_cli_with_nick_name(recv_vector[1]))
		ft_send(ERR_NICKNAMEINUSE, ":Nickname is already in use", cli, true);
	else
	{
		ft_send(RPL_WELCOME, recv_vector[1] + " :Welcome to the ft_irc Network " + recv_vector[1] , cli, false);
		ft_send(RPL_YOURHOST, recv_vector[1] + " :Your nickname is " + recv_vector[1] + ", running version", cli, false);
		cli->set_nick_name(recv_vector[1]);
	}
}
