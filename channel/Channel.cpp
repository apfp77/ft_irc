#include "Channel.hpp"
#include "ft_utils.hpp"

Channel::Channel(std::string &ch_name, Client *cli)
{
	this->ch_name = ch_name;
	this->cli_lst.insert(cli);
	this->gm_lst.insert(cli);
}


void Channel::insert_cli(Client *cli) {	this->cli_lst.insert(cli);}

void Channel::delete_cli(Client *cli) 
{
	std::set<Client *>::iterator it = this->cli_lst.find(cli);
	if (it != this->cli_lst.end())
		this->cli_lst.erase(it);
}
void Channel::insert_cli_gm(Client *cli) {	this->gm_lst.insert(cli);}

void Channel::delete_cli_gm(Client *cli) 
{
	std::set<Client *>::iterator it = this->gm_lst.find(cli);
	if (it != this->gm_lst.end())
		this->gm_lst.erase(it);
}

void Channel::set_passwd(std::string &passwd) {	this->passwd = passwd; }
std::string Channel::get_passwd() const { return (this->passwd);}

std::string Channel::get_ch_name() const {	return (this->ch_name); }

std::set<Client *>::size_type Channel::get_cli_lst_size() const { return (this->cli_lst.size()); }

Client* Channel::find_cli_in_ch(Client *cli)
{
	std::set<Client *>::iterator it = this->cli_lst.find(cli);
	if (it != cli_lst.end())
		return ((*it));
	return (NULL);
}

void Channel::send_to_ch(std::string &message)
{
	std::set<Client *>::iterator it = this->cli_lst.begin();
	for (; it != this->cli_lst.end(); ++it)
	{
		ft_send("", message, (*it), false);
	}
}
