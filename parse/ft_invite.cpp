#include "parse.hpp"

/*
	Command: INVITE
  Parameters: <nickname> <channel>

	ERR_NEEDMOREPARAMS (461): 매개변수가 부족한경우
	ERR_NOSUCHCHANNEL (403): 채널이 없는경우
	ERR_NOTONCHANNEL (442): 초대하는 클라이언트가 채널에 없는경우
	ERR_USERONCHANNEL (443): 초대하려는 클라이언트가 이미 채널에 있는 경우
	ERR_CHANOPRIVSNEEDED (482): 운영자가 아닌데 초대하려는 경우
	RPL_INVITING (341): 해당 닉네임을 가진 사용자가 초대됨을 알림
*/
void ft_invite(std::vector<std::string> &recv_vector, Client *cli, Server &serv)
{
	if (recv_vector.size() < 3)
	{
		ft_send(ERR_NEEDMOREPARAMS, cli->get_nick_name() + " INVITE " + ":Not enough parameters", cli);
		return ;
	}
	Channel *ch = serv.find_ch_with_ch_name(recv_vector[2]);
	if (ch == NULL)
	{
		ft_send(ERR_NOSUCHCHANNEL, cli->get_nick_name() + " " + recv_vector[2] +   " :No such channel", cli);
		return ;
	}
	if (ch->find_cli_in_ch(cli) == NULL)
	{
		ft_send(ERR_NOTONCHANNEL, cli->get_nick_name() + " " + recv_vector[2] +   " :You're not on that channel", cli);
		return ;
	}
	Client *user = ch->find_cli_in_ch_by_str(recv_vector[1]);
	if (user != NULL)
	{
		ft_send(ERR_USERONCHANNEL, cli->get_nick_name() + " " + recv_vector[1] + " " + recv_vector[2] + ":is already on channel", cli);
		return ;
	}
	// 1. ! 추가
	if (!ch->find_cli_in_gm_lst(cli))
	{
		ft_send(ERR_CHANOPRIVSNEEDED, cli->get_nick_name() + " " + recv_vector[2] + " :You're not channel operator", cli);
		return ;
	}
	//  서버에 유저가 있는지 확인
	user = serv.find_cli_with_nick_name(recv_vector[1]);
	if (user == NULL)
	{
		//  "<client> <nickname> :No such nick/channel"
		ft_send(ERR_NOSUCHNICK, cli->get_nick_name() + " " + recv_vector[1] + " :No such nick/channel", cli);
		return ;
	}
	ch->insert_invite_cli(user);
	std::string message = ":";
	message += SERVER_NAME;
	message += RPL_INVITING;
	message += " " + cli->get_nick_name()+ "!ft_irc@ft_irc" + " " + recv_vector[1] + " " + recv_vector[2];
	ch->all_send_to_ch(message);
	//노티스 추가해야할까?

	//초대받은 클라이언트한테 메세지 보내기
	message = ": " + cli->get_nick_name() + " INVITE " + recv_vector[1] + " " + recv_vector[2];
	ft_send("", message, user);
}
