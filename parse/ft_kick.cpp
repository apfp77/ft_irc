#include "parse.hpp"

#define siz_t std::vector<std::string>::size_type

typedef struct s_err_box
{
	std::string err_code;
	std::string err_message;
} t_err_box;

void ft_kick(std::vector<std::string> &recv_vector, Client *cli, Server &serv);
static void delete_server_in_channel(Channel *channel, Server &serv);
static void send_message_channel_member(std::vector<std::string> &recv_vector, Channel *channel, std::vector<std::string> users, Client *cli);
static std::string merge_send_message(std::string kick_channel_message, std::string user, std::string explain_message);
static std::string merge_kick_channel_message(std::vector<std::string> &recv_vector);
static std::string merge_explain_message(std::vector<std::string> &recv_vector, std::string cli_nick);
static std::vector<std::string> check_kick_target_in_channel(Channel *channel, Client *cli, std::string recv_vector);

static void check_parameter(std::vector<std::string> &recv_vector, Client *cli);
static Channel *check_exist_channel(std::string recv_vector, Server &serv, Client *cli);

static void check_me_belong_channel(Channel *channel, Client *cli);
static void check_me_operator_in_channel(Channel *channel, Client *cli);

void ft_kick(std::vector<std::string> &recv_vector, Client *cli, Server &serv)
{
	try
	{

		check_parameter(recv_vector, cli);
		Channel *channel = check_exist_channel(recv_vector[1], serv, cli);

		check_me_belong_channel(channel, cli);
		check_me_operator_in_channel(channel, cli);
		std::vector<std::string> users = check_kick_target_in_channel(channel, cli, recv_vector[3]);
		send_message_channel_member(recv_vector, channel, users, cli);
		delete_server_in_channel(channel, serv);
	}
	catch(t_err_box err_box)
	{
		ft_send(err_box.err_code, err_box.err_message, cli, true);
	}
}

static void delete_server_in_channel(Channel *channel, Server &serv)
{
	if(channel->get_cli_lst_size() == 0)
	{
		serv.delete_ch(channel);
		delete channel;
	}
}

static void send_message_channel_member(std::vector<std::string> &recv_vector, Channel *channel, std::vector<std::string> users, Client *cli)
{
	int flag = 1;
	siz_t end = users.size();
	std::string cli_nick = cli->get_nick_name();
	if (end == 0)
		return ;
	std::string explain_message = merge_explain_message(recv_vector, cli_nick);
	std::string kick_channel_message = merge_kick_channel_message(recv_vector);
	for (siz_t i = 0; i < end; i++)
	{
		std::string send_message = merge_send_message(kick_channel_message, users[i], explain_message);
		channel->all_send_to_ch(send_message);
		if (flag == 1)
			channel->send_to_ch(send_message, cli);
		channel->delete_gm_cli_and_cli(channel->find_cli_in_ch_by_str(users[i]));
		if (flag == 1 && users[i].compare(cli_nick) == 0)
			flag = 0;
	}
}

static std::string merge_send_message(std::string kick_channel_message, std::string user, std::string explain_message)
{
	std::string send_message;

	send_message.append(kick_channel_message);
	send_message.append(user);
	send_message.append(" ");
	send_message.append(explain_message);

	return (send_message);
}

static std::string merge_kick_channel_message(std::vector<std::string> &recv_vector)
{
	std::string message;

	message.append(recv_vector[0]);
	message.append(" ");
	message.append(recv_vector[1]);
	message.append(" ");

	return (message);
}

static std::string merge_explain_message(std::vector<std::string> &recv_vector, std::string cli_nick)
{
	std::string message;

	int max = recv_vector.size();
	if (max == 3)
		return (cli_nick);
	message.append(":");
	for (int i = 3; i < max; i++)
	{
		message.append(recv_vector[i]);
		if (!(i == max - 1))
			message.append(" ");
	}
	return (message);
}

static std::vector<std::string> check_kick_target_in_channel(Channel *channel, Client *cli, std::string recv_vector)
{

	std::vector<std::string> users = ft_split(recv_vector, ",");
	siz_t end = users.size();
	for (siz_t i = 0; i < end;)
	{
		try{
			if (channel->find_cli_in_ch_by_str(users[i]) == NULL)
			{
				users.erase(users.begin() + i);
				t_err_box err_box;
				err_box.err_code = ERR_USERNOTINCHANNEL;

				err_box.err_message.append(cli->get_nick_name());
				err_box.err_message.append(" ");
				err_box.err_message.append(channel->get_ch_name());
				err_box.err_message.append(" :They aren't on that channel");

				throw (err_box);
			}
			i++;
		}
		catch(t_err_box err_box)
		{
			ft_send(err_box.err_code, err_box.err_message, cli, true);
		}
	}
	return (users);
}


static void check_parameter(std::vector<std::string> &recv_vector, Client *cli)

{
	if (recv_vector.size() <= 2)
	{
		t_err_box err_box;
		err_box.err_code = ERR_NEEDMOREPARAMS;

		err_box.err_message.append(cli->get_nick_name());
		err_box.err_message.append(" /KICK");
		err_box.err_message.append(" :Not enough parameters");

		throw (err_box);
	}
}


static Channel *check_exist_channel(std::string recv_vector, Server &serv, Client *cli)

{
	Channel *channel = serv.find_ch_with_ch_name(recv_vector);

	if (channel == NULL)
	{
		t_err_box err_box;
		err_box.err_code = ERR_NOSUCHCHANNEL;

		err_box.err_message.append(cli->get_nick_name());
		err_box.err_message.append(" ");
		err_box.err_message.append(recv_vector);
		err_box.err_message.append(" :No such channel");

		throw (err_box);
	}
	return (channel);
}

static void check_me_belong_channel(Channel *channel, Client *cli)
{
	if (channel->find_cli_in_ch(cli) == NULL)
	{
		t_err_box err_box;
		err_box.err_code = ERR_NOTONCHANNEL;

		err_box.err_message.append(cli->get_nick_name());
		err_box.err_message.append(" ");
		err_box.err_message.append(channel->get_ch_name());
		err_box.err_message.append(" :You're not on that channel");

		throw (err_box);
	}
}

static void check_me_operator_in_channel(Channel *channel, Client *cli)
{
	if (channel->find_cli_in_gm_ch(cli) == NULL)
	{
		t_err_box err_box;
		err_box.err_code = ERR_CHANOPRIVSNEEDED;

		err_box.err_message.append(cli->get_nick_name());
		err_box.err_message.append(" ");
		err_box.err_message.append(channel->get_ch_name());
		err_box.err_message.append(" :You're not channel operator");

		throw (err_box);
	}
}