#include "parse.hpp"

#define siz_t std::vector<std::string>::size_type

typedef struct s_err_box
{
	std::string message;
	std::string err_message;
} t_err_box;

class c_part_data
{
	private:
		std::vector<std::string> channel_names;
		std::string cli_nick;
		std::string explain;
		siz_t ch_max;
		siz_t max;
	public:
		t_err_box err_box;
		c_part_data(std::vector<std::string> &recv_vector, Client *cli)
		{
			this->cli_nick = cli->get_nick_name();
			this->max = recv_vector.size();
			this->err_box.message = "";
			this->err_box.err_message = "";
			if (this->max <= 1)
			{
				err_box.err_message.append(": ");
				err_box.err_message.append(ERR_NEEDMOREPARAMS);
				err_box.err_message.append(" ");
				err_box.err_message.append(cli_nick);
				err_box.err_message.append(" PART");
				err_box.err_message.append(" :Not enough parameters\r\n");
				throw(err_box.err_message);
			}
			else
			{
				channel_names = ft_split(recv_vector[1], ",");
				ch_max = channel_names.size();
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
		siz_t get_channel_size() {return (this->ch_max);}
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

static bool check_exist_channal(Channel *channel, c_part_data &p_data, std::string &channel_name);
static bool check_channel_in_cil(Channel *channel, Client *cli, c_part_data &p_data, std::string &channel_name);

void ft_part(std::vector<std::string> &recv_vector, Client *cli, Server &serv)
{
	try
	{
		c_part_data p_data = c_part_data(recv_vector, cli);
		siz_t max = p_data.get_channel_size();

		for (siz_t i = 0; i < max; i++)
		{
			std::string channel_name = p_data.get_channel_name(i);
			Channel *channel = serv.find_ch_with_ch_name(channel_name);
			if (check_exist_channal(channel, p_data, channel_name))
				continue;
			if (check_channel_in_cil(channel, cli, p_data, channel_name))
				continue;
			std::string send_message = p_data.merge_send_message(channel_name);
			channel->send_to_ch(send_message, cli);
			channel->delete_gm_cli_and_cli(cli);
			if (channel->get_cli_lst_size() == 0)
			{
				serv.delete_ch(channel);
				delete channel;
			}
		}
		ft_send("", p_data.err_box.message + p_data.err_box.err_message, cli, false);
	}
	catch(const char *e)
	{
		ft_send("", e, cli, false);
		return ;
	}
}

bool check_exist_channal(Channel *channel, c_part_data &p_data, std::string &channel_name)
{
	if (channel == NULL)
	{
		p_data.err_box.err_message.append(":");
		p_data.err_box.err_message.append(p_data.get_nick_name());
		p_data.err_box.err_message.append(" ");
		p_data.err_box.err_message.append(ERR_NOSUCHCHANNEL);
		p_data.err_box.err_message.append(" ");
		p_data.err_box.err_message.append(p_data.get_nick_name());
		p_data.err_box.err_message.append(" ");
		p_data.err_box.err_message.append(channel_name);
		p_data.err_box.err_message.append(" :No such channel\r\n");
		return (true);
	}
	return (false);
}

bool check_channel_in_cil(Channel *channel, Client *cli, c_part_data &p_data, std::string &channel_name)
{
	if (channel->find_cli_in_ch(cli) == NULL)
	{
		p_data.err_box.err_message += ":";
		p_data.err_box.err_message += ERR_USERONCHANNEL;
		p_data.err_box.err_message += p_data.get_nick_name();
		p_data.err_box.err_message += " ";
		p_data.err_box.err_message += p_data.get_nick_name();
		p_data.err_box.err_message += " ";
		p_data.err_box.err_message.append(" ");
		p_data.err_box.err_message.append(channel_name);
		p_data.err_box.err_message.append(" :You're not on that channel\r\n");
		return (true);
	}
	return (false);
}
