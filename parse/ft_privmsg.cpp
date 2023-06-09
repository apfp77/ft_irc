#include "parse.hpp"

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
void ft_privmsg(std::vector<std::string> &recv_vector, Client *cli, Server &serv)
{
	std::vector<std::string>::size_type recv_size = recv_vector.size();
	if (recv_size < 3)
	{
		if (recv_size < 2)
			ft_send(ERR_NORECIPIENT, ":No recipient given", cli);
		else
			ft_send(ERR_NOTEXTTOSEND, ":No text to send", cli);
		return ;
	}

	std::vector<std::string> target_split = ft_split(recv_vector[1], ",");
	for (std::vector<std::string>::size_type j = 0; j < target_split.size(); j++)
	{
		if (return_string_type(target_split[j]) == CHANNEL)
		{
			std::string ch_name = target_split[j];
			Channel *privmsg_ch = serv.find_ch_with_ch_name(ch_name);
			
			if (privmsg_ch == NULL 
				|| privmsg_ch->find_cli_in_ch(cli) == NULL)
			{
				ft_send(ERR_CANNOTSENDTOCHAN, ":Cannot send to channel", cli);
				return ;
			}
			
			std::string message = ":" + cli->get_nick_name() + "!ft_irc@ft_irc" + " PRIVMSG " + ch_name + " ";
			for (std::vector<std::string>::size_type i = 2; i < recv_size; i++)
				message += recv_vector[i];
			privmsg_ch->send_to_ch(message, cli);
		}
		else if (return_string_type(target_split[j]) == USER)
		{
			std::string nick_name = target_split[j];
			Client *privmsg_cli = serv.find_cli_with_nick_name(nick_name);
			if (privmsg_cli == NULL)
			{
				ft_send(ERR_NOSUCHNICK, ":No such nick/channel", cli);
				return ;
			}
			std::string message = ":" + cli->get_nick_name() + " PRIVMSG " + nick_name + " ";
			for (std::vector<std::string>::size_type i = 2; i < recv_size; i++)
				message += recv_vector[i];
			ft_send("", message, privmsg_cli);
		}
	}
}