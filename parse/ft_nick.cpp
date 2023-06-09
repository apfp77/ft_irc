#include "parse.hpp"

/*
	1. ERR_NOICKGIVEN: 매개변수가 없는경우
	2. ERR_ERRONEUSNICKNAME: nick에 허용하지 않는 문자가 포함될 경우
	3. ERR_NICKNAMEINUSE: 이미 동일한 nick이 존재할경우
	4. ERR_NICKCOLLISION: 다른 서버에 nick이 존재할경우 (우리 서버는 하나이므로 처리하지 않음)
*/
bool ft_connect_nick(std::vector<std::string> &recv_vector, Client *cli, Server &serv)
{
	if (recv_vector.size() == 2 
			&& string_isalnum(recv_vector[1]) 
			&& !serv.find_cli_with_nick_name(recv_vector[1]) 
			&& cli->pass_flag == true)
	{
		ft_send(RPL_WELCOME, recv_vector[1] + " :Welcome to the ft_irc Network " + recv_vector[1] , cli);
		ft_send(RPL_YOURHOST, recv_vector[1] + " :Your nickname is " + recv_vector[1] + ", running version", cli);
		cli->set_nick_name(recv_vector[1]);
		cli->nick_flag = true;
		return (false);
	}
	if (recv_vector.size() != 2)
		ft_send(ERR_NONICKNAMEGIVEN, ":No nickname given", cli);
	else if (!string_isalnum(recv_vector[1]))
		ft_send(ERR_ERRONEUSNICKNAME, ":Erroneus nickname", cli);
	else if (serv.find_cli_with_nick_name(recv_vector[1]))
		ft_send(ERR_NICKNAMEINUSE, ":Nickname is already in use", cli);
	if (cli->pass_flag == false)
	{
		pollfd *p_fd = serv.find_fds(cli->get_socket());
		p_fd->revents = POLLHUP;
	}
	return (true);
}

void ft_nick(std::vector<std::string> &recv_vector, Client *cli, Server &serv)
{
	if (recv_vector.size() != 2)
		ft_send(ERR_NONICKNAMEGIVEN, ":No nickname given", cli);
	else if (!string_isalnum(recv_vector[1]))
		ft_send(ERR_ERRONEUSNICKNAME, ":Erroneus nickname", cli);
	else if (serv.find_cli_with_nick_name(recv_vector[1]))
		ft_send(ERR_NICKNAMEINUSE, ":Nickname is already in use", cli);
	else
	{
		ft_send("", cli->get_nick_name() + " NICK :" + recv_vector[1], cli);
		std::set<Channel *> channels = serv.find_cli_in_channels(cli);
		std::set<Channel *>::iterator it= channels.begin();
		std::string message = ":" + cli->get_nick_name()+ "!ft_irc@ft_irc" + " NICK :" + recv_vector[1];
		for (; it != channels.end(); ++it)
			(*it)->all_send_to_ch(message);
		cli->set_nick_name(recv_vector[1]);
	}
}
