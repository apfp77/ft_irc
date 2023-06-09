#include "parse.hpp"

#define siz_t std::vector<std::string>::size_type

typedef struct s_msg_box
{
	std::string message;
	std::string err_message;
} t_msg_box;

class c_part_data
{
	private:
		std::vector<std::string> channel_names;
		std::string cli_nick;
		std::string explain;
		siz_t ch_max;
		siz_t max;
	public:
		t_msg_box msg_box;
		c_part_data(std::vector<std::string> &recv_vector, Client *cli)
		{
			this->cli_nick = cli->get_nick_name();
			this->max = recv_vector.size();
			this->msg_box.message = "";
			this->msg_box.err_message = "";
			if (this->max <= 1)
			{
				msg_box.err_message.append(": ");
				msg_box.err_message.append(ERR_NEEDMOREPARAMS);
				msg_box.err_message.append(" ");
				msg_box.err_message.append(cli_nick);
				msg_box.err_message.append(" PART");
				msg_box.err_message.append(" :Not enough parameters\r\n");
				throw(msg_box.err_message.c_str());
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
		
		void part_success_message(std::string &channel_name) 
		{
			msg_box.message.append(":");
			msg_box.message.append(this->cli_nick);
			msg_box.message.append("!ft_irc@ft_irc");
			msg_box.message.append(" ");
			msg_box.message.append("PART");
			msg_box.message.append(" ");
			msg_box.message.append(channel_name);
			msg_box.message.append(" ");
			msg_box.message.append(this->explain);
			msg_box.message.append("\r\n");
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
			p_data.part_success_message(channel_name);
			channel->send_to_ch(p_data.msg_box.message, cli);
			channel->delete_gm_cli_and_cli(cli);
			if (channel->get_cli_lst_size() == 0)
			{
				serv.delete_ch(channel);
				delete channel;
			}
		}
		ft_send("", p_data.msg_box.message + p_data.msg_box.err_message, cli);
	}
	catch(const char *e)
	{
		ft_send("", e, cli);
		return ;
	}
}

bool check_exist_channal(Channel *channel, c_part_data &p_data, std::string &channel_name)
{
	if (channel == NULL)
	{
		p_data.msg_box.err_message.append(":");
		p_data.msg_box.err_message.append(ERR_NOSUCHCHANNEL);
		p_data.msg_box.err_message.append(" ");
		p_data.msg_box.err_message.append(p_data.get_nick_name());
		p_data.msg_box.err_message.append(" ");
		p_data.msg_box.err_message.append(channel_name);
		p_data.msg_box.err_message.append(" :No such channel\r\n");
		return (true);
	}
	return (false);
}

bool check_channel_in_cil(Channel *channel, Client *cli, c_part_data &p_data, std::string &channel_name)
{
	if (channel->find_cli_in_ch(cli) == NULL)
	{
		p_data.msg_box.err_message.append(":");
		p_data.msg_box.err_message.append(ERR_NOTONCHANNEL);
		p_data.msg_box.err_message.append(" ");
		p_data.msg_box.err_message.append(p_data.get_nick_name());
		p_data.msg_box.err_message.append(" ");
		p_data.msg_box.err_message.append(channel_name);
		p_data.msg_box.err_message.append(" :You're not on that channel\r\n");
		return (true);
	}
	return (false);
}
