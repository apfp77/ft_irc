#include "parse.hpp"

bool ft_pass(std::vector<std::string> &recv_vector, Client *cli, Server &serv)
{
	if (recv_vector.size() == 2 && !recv_vector[1].compare(serv.get_passwd()))
	{
		cli->pass_flag = true;
		return (false);
	}
	if (recv_vector.size() != 2)
	{
		ft_send(ERR_NEEDMOREPARAMS, ":Not enough parameters", cli);
		serv.check_pass_flag_cli_exit(cli);
	}
	else
	{
		serv.password_err_message(cli);
		serv.check_pass_flag_cli_exit(cli);
	}
	return (true);
}
