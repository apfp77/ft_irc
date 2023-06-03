#include "Mode.hpp"

Mode::Mode()
{
	mode_invite = false;
	mode_topic = false;
	mode_key = false;
	mode_limit = false;
	passwd = "";
	cli_limit = 0;
}

bool Mode::set_mode_invite(bool set) { this->mode_invite = set; }
bool Mode::set_mode_topic(bool set) { this->mode_topic = set; }
bool Mode::set_mode_key(bool set) { this->mode_key = set; }
bool Mode::set_mode_limit(bool set) {this->mode_limit = set; }	
void Mode::set_cli_limit(int size) { this->cli_limit = size;}
void Mode::set_passwd(std::string &passwd) { this->passwd = passwd; }

bool Mode::get_mode_invite() const { return (this->mode_invite); }
bool Mode::get_mode_topic() const { return (this->mode_topic); }
bool Mode::get_mode_key() const { return (this->mode_key); }
bool Mode::get_mode_limit() const { return (this->mode_limit); }
int	Mode::get_cli_limit() const { return (this->cli_limit); }
std::string Mode::get_passwd() const { return (this->passwd); }


void Mode::insert_invite_cli(Client *cli)
{
	this->invite_cli_set.insert(cli);
}

void Mode::delete_invite_cli(Client *cli)
{
	std::set<Client *>::iterator it = this->cli_lst.find(cli);
	if (it != this->cli_lst.end())
		this->cli_lst.erase(it);
}

Mode::~Mode()
{

}
