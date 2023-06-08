#include "parse.hpp"

#define siz_t std::vector<std::string>::size_type

typedef struct s_err_box
{
	std::string err_code;
	std::string err_message;
} t_err_box;

class c_part_data
{
	private:
		std::vector<std::string> channel_names;
		std::string cli_nick;
		std::string explain;
		siz_t max;
	public:
		c_part_data(std::vector<std::string> &recv_vector, Client *cli)
		{
			t_err_box err_box;
			this->cli_nick = cli->get_nick_name();
			this->max = recv_vector.size();
			if (this->max <= 1)
			{
				err_box.err_code = ERR_NEEDMOREPARAMS;
				err_box.err_message = cli_nick;
				err_box.err_message.append(" PART");
				err_box.err_message.append(" :Not enough parameters");
				throw (err_box);
			}
			else
			{
				channel_names = ft_split(recv_vector[1], ",");
				if (this->max == 3 && recv_vector[2].length() == 1)
					this->explain = recv_vector[2].append(cli_nick);
				else
				{
					for (size_t i = 2; i < this->max; i++)
						this->explain.append(recv_vector[i]);
				}
			}
		}
		~c_part_data(){}
		siz_t get_size(){ return (this->max); }
		std::string get_channel_name(siz_t i){ return (this->channel_names[i]); }
		std::string get_nick_name(){ return (this->cli_nick); }
		std::string merge_send_message(std::string &channel_name) 
		{
			std::string send_message;
			send_message.append(":");
			send_message.append(this->cli_nick);
			send_message.append(" ");
			send_message.append("PART");
			send_message.append(" ");
			send_message.append(channel_name);
			send_message.append(" ");
			send_message.append(this->explain);
			return (send_message);
		}
		
};

static void check_exist_channal(Channel *channel, c_part_data &p_data, std::string &channel_name);
static void check_channel_in_cil(Channel *channel, Client *cli, c_part_data &p_data, std::string &channel_name);

void ft_part(std::vector<std::string> &recv_vector, Client *cli, Server &serv)
{
	try
	{
		c_part_data p_data = c_part_data(recv_vector, cli);
		siz_t max = p_data.get_size();
		for (size_t i = 0; i < max; i++)
		{
			try
			{
				std::string channel_name = p_data.get_channel_name(i);
				Channel *channel = serv.find_ch_with_ch_name(channel_name);
				check_exist_channal(channel, p_data, channel_name);
				check_channel_in_cil(channel, cli, p_data, channel_name);
				std::string send_message = p_data.merge_send_message(channel_name);
				channel->all_send_to_ch(send_message);
				channel->delete_gm_cli_and_cli(cli);
				if (channel->get_cli_lst_size() == 0)
				{
					serv.delete_ch(channel);
					delete channel;
				}
			}
			catch (t_err_box err_box)
			{
				ft_send(err_box.err_code, err_box.err_message, cli, true);
			}
		}
	}
	catch (t_err_box err_box)
	{
		ft_send(err_box.err_code, err_box.err_message, cli, true);
	}
}

void check_exist_channal(Channel *channel, c_part_data &p_data, std::string &channel_name)
{
	if (channel == NULL)
	{
		t_err_box err_box;
		err_box.err_code = ERR_NOSUCHCHANNEL;
		err_box.err_message = p_data.get_nick_name();
		err_box.err_message.append(" ");
		err_box.err_message.append(channel_name);
		err_box.err_message.append(" :No such channel");
		throw (err_box);
	}
}

void check_channel_in_cil(Channel *channel, Client *cli, c_part_data &p_data, std::string &channel_name)
{
	if (channel->find_cli_in_ch(cli) == NULL)
	{
		t_err_box err_box;
		err_box.err_code = ERR_NOSUCHCHANNEL;
		err_box.err_message = p_data.get_nick_name();
		err_box.err_message.append(" ");
		err_box.err_message.append(channel_name);
		err_box.err_message.append(" :You're not on that channel");
		throw (err_box);
	}
}
