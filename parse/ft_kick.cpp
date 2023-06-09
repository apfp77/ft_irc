#include "parse.hpp"

#define siz_t std::vector<std::string>::size_type

typedef struct s_err_box
{
	std::string err_code;
	std::string err_message;
} t_err_box;

typedef struct s_kick_data
{
	std::string nick;
	std::string explain;
	std::string channel_name;
	std::string send_first_mention;
	Channel *channel;
	bool check_parameter;
	bool exist_channel;
	bool im_operator;
	bool im_in_channel;
	siz_t parameters;
	std::vector<std::string> erase_user_name;
} t_kick_data;

// ft_kick 생성, 빌드
void ft_kick(std::vector<std::string> &recv_vector, Client *cli, Server &serv);
static void make_up_data(std::vector<std::string> &recv_vector, Client *cli, Server &serv, t_kick_data &t_data);
static void init_k_data(t_kick_data &k_data);

// 오류 케이스
static void check_parameter(t_kick_data &k_data);
static void check_exist_channel(t_kick_data &k_data, std::string &str);
static void check_me_operator_in_channel(t_kick_data &k_data);
static void check_me_belong_channel(t_kick_data &k_data);
static void check_kick_target_in_channel(t_kick_data &k_data, std::string &user);

// 정상 전송
static void delete_server_in_channel(t_kick_data &k_data, Server &serv);
static void send_message_channel_member(t_kick_data &k_data, std::string &user);

// 나머지 문자열 합치는 부분
static std::string merge_send_message(std::string kick_channel_message, std::string user, std::string explain_message);
static std::string merge_kick_channel_message(std::vector<std::string> &recv_vector);
static std::string merge_explain_message(std::vector<std::string> &recv_vector, std::string cli_nick, int max);


void ft_kick(std::vector<std::string> &recv_vector, Client *cli, Server &serv)
{
	t_kick_data k_data;

	make_up_data(recv_vector, cli, serv, k_data);
	try
	{
		check_parameter(k_data);
		try
		{
			std::vector<std::string>::iterator it = k_data.erase_user_name.begin();
			for (; it != k_data.erase_user_name.end(); ++it)
			{
				check_exist_channel(k_data, recv_vector[1]);
				check_me_operator_in_channel(k_data);
				check_me_belong_channel(k_data);
				check_kick_target_in_channel(k_data, (*it));
				send_message_channel_member(k_data, (*it));
				delete_server_in_channel(k_data, serv);
			}
		}
		catch(t_err_box err_box)
		{
			ft_send(err_box.err_code, err_box.err_message, cli);
		}
	}
	catch(t_err_box err_box)
	{
		ft_send(err_box.err_code, err_box.err_message, cli);
	}
}

static void make_up_data(std::vector<std::string> &recv_vector, Client *cli, Server &serv, t_kick_data &k_data)
{
	init_k_data(k_data);
	k_data.nick = cli->get_nick_name();
	k_data.parameters = recv_vector.size();
	if (k_data.parameters <= 2)
		k_data.check_parameter = false;
	else
	{
		k_data.channel = serv.find_ch_with_ch_name(recv_vector[1]);
		if (k_data.channel == NULL)
			k_data.exist_channel = false;
		else
		{
			k_data.channel_name = k_data.channel->get_ch_name();
			if (k_data.channel->find_cli_in_ch(cli) == NULL)
				k_data.im_in_channel = false;
			if (k_data.channel->find_cli_in_gm_ch(cli) == NULL)
				k_data.im_operator = false;
		}
			k_data.erase_user_name = ft_split(recv_vector[2], ",");
			k_data.send_first_mention = ":"+ k_data.nick + "!ft_irc@ft_irc " + merge_kick_channel_message(recv_vector);
			k_data.explain = merge_explain_message(recv_vector, k_data.nick, k_data.parameters);
	}
}

static void init_k_data(t_kick_data &k_data)
{
	k_data.channel = NULL;
	k_data.check_parameter = true;
	k_data.exist_channel = true;
	k_data.explain = "";
	k_data.im_in_channel = true;
	k_data.im_operator = true;
	k_data.nick = "";
}

static void delete_server_in_channel(t_kick_data &k_data, Server &serv)
{
	if(k_data.channel->get_cli_lst_size() == 0)
	{
		serv.delete_ch(k_data.channel);
		delete k_data.channel;
		k_data.exist_channel = false;
	}
}

static void send_message_channel_member(t_kick_data &k_data, std::string &user)
{
	std::string send_message = merge_send_message(k_data.send_first_mention, user, k_data.explain);
	k_data.channel->all_send_to_ch(send_message);
	k_data.channel->delete_gm_cli_and_cli(k_data.channel->find_cli_in_ch_by_str(user));
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

static std::string merge_explain_message(std::vector<std::string> &recv_vector, std::string cli_nick, int max)
{
	std::string message = "";

	if (max == 4 && recv_vector[3].length() == 1)
	{
		message.append(recv_vector[3]);
		if (recv_vector[3].compare(":") == 0)
		{
			message.append(cli_nick);
		}
		return (message);
	}
	if (max >= 4)
	{
		for (int i = 3; i < max; i++)
		{
			message.append(recv_vector[i]);
			if (!(i == max - 1))
				message.append(" ");
		}
	}
	return (message);
}

static void check_kick_target_in_channel(t_kick_data &k_data, std::string &user)
{
	if (k_data.channel->find_cli_in_ch_by_str((user)) == NULL)
	{
		t_err_box err_box;
		err_box.err_code = ERR_USERNOTINCHANNEL;
		err_box.err_message.append(k_data.nick);
		err_box.err_message.append(" ");
		err_box.err_message.append(user);
		err_box.err_message.append(" ");
		err_box.err_message.append(k_data.channel_name);
		err_box.err_message.append(" :They aren't on that channel");
		throw (err_box);
	}
}


static void check_parameter(t_kick_data &k_data)

{
	if (k_data.check_parameter == false)
	{
		t_err_box err_box;
		err_box.err_code = ERR_NEEDMOREPARAMS;
		err_box.err_message.append(k_data.nick);
		err_box.err_message.append(" KICK");
		err_box.err_message.append(" :Not enough parameters");
		throw (err_box);
	}
}


static void check_exist_channel(t_kick_data &k_data, std::string &str)
{
	if (k_data.exist_channel == false)
	{
		t_err_box err_box;
		err_box.err_code = ERR_NOSUCHCHANNEL;
		err_box.err_message.append(k_data.nick);
		err_box.err_message.append(" ");
		err_box.err_message.append(str);
		err_box.err_message.append(" :No such channel");
		throw (err_box);
	}
}

static void check_me_belong_channel(t_kick_data &k_data)
{
	if (k_data.im_in_channel == false)
	{
		t_err_box err_box;
		err_box.err_code = ERR_NOTONCHANNEL;
		err_box.err_message.append(k_data.nick);
		err_box.err_message.append(" ");
		err_box.err_message.append(k_data.channel_name);
		err_box.err_message.append(" :You're not on that channel");

		throw (err_box);
	}
}

static void check_me_operator_in_channel(t_kick_data &k_data)
{
	if (k_data.im_operator == false)
	{
		t_err_box err_box;
		err_box.err_code += ERR_CHANOPRIVSNEEDED;
		err_box.err_message.append(k_data.nick);
		err_box.err_message.append(" ");
		err_box.err_message.append(k_data.channel_name);
		err_box.err_message.append(" :You're not channel operator");

		throw (err_box);
	}
}