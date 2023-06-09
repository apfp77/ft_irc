#include "parse.hpp"

void send_input_cli_in_ch(Channel *ch, Client *cli)
{
	std::string ch_message = ":" + cli->get_nick_name() + "!ft_irc@ft_irc" + " JOIN :" + ch->get_ch_name();
	ch->all_send_to_ch(ch_message);
}

/*
	1. 사용자 목록을 한줄로 합치는 작업을 합니다
	2. 운영자라면 운영자를 뜻하는 @를 붙여서 보냅니다
*/
std::string input_operator_prefix(Channel *ch, std::vector<std::string> &users_lst)
{
	std::vector<std::string>::iterator it = users_lst.begin();
	std::string ret;
	for(;it != users_lst.end();++it)
	{
		Client *gm_true = ch->find_cli_in_gm_lst_by_str((*it));
		if (gm_true != NULL)
			ret += "@";
		ret += (*it);
		ret += " ";
	}
	return (ret);
}

void send_rpl_namreply(std::string &ch_name, Client *cli, std::string &users)
{
	std::string message = ":";
	message += SERVER_NAME;
	message += RPL_NAMREPLY;
	message +=  " " + cli->get_nick_name() + " = " + ch_name + " :" + users;
	ft_send("", message + users, cli);
}

void send_names_message(std::string &ch_name, Client *cli)
{	
	std::string message = ":";
	message += SERVER_NAME;
	message += RPL_ENDOFNAMES;
	message += " " + cli->get_nick_name() + " " + ch_name + " :End of /NAMES list";
	ft_send("", message, cli);
}

/*
	1. 채널에 사용자가 추가되었음을 알립니다
	2. 매개변수로 들어온 사용자에게 현제 채널의 사용자 목록을 알립니다
*/
void ret_join_after_names_message(Channel *ch, Client *cli)
{
	send_input_cli_in_ch(ch, cli);
	std::vector<std::string> users_lst = ch->get_cli_names_in_ch();
	std::string users = input_operator_prefix(ch, users_lst);
	std::string ch_name = ch->get_ch_name();
	send_rpl_namreply(ch_name, cli, users);
	send_names_message(ch_name, cli);
}

void ft_names(std::vector<std::string> &recv_vector, Client *cli, Server &serv)
{
	if (recv_vector.size() < 1)
	{
		ft_send(ERR_NEEDMOREPARAMS, ":Not enough parameters", cli);
		return ;
	}
	(void)serv;
	
}
