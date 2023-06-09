#include "Server.hpp"
//stringstream
#include <sstream>

#include <cstdlib>

bool Server::is_num(char *s)
{
	int i = 0;
	if (s[i] == '-' || !std::isdigit(s[i++]))
		return (false);
	while (s[i] && std::isdigit(s[i]))
		i++;
	if (s[i] == '\0')
		return (true);
	return (false);
}

Server::Server(char *srv_port, char *passwd) : passwd(passwd)
{
		if (this->is_num(srv_port) == false)
			throw ("is not num");
		std::stringstream ss(srv_port);
		ss >> this->srv_port;
		if (this->srv_port > 65535)
			throw("is over port max");
		std::string str = passwd;
		if (str.empty())
			throw("empty passwd");
		int len = str.length();
		if (len < 1 || len > 11)
			throw("len_worng passwd");
}

void Server::parse_map_init()
{
	parse_map["PING"] = 1;
	parse_map["PASS"] = 2;
	parse_map["NICK"] = 3;
	parse_map["NAMES"] = 4;
	parse_map["PRIVMSG"] = 5;
	parse_map["TOPIC"] = 6;
	parse_map["JOIN"] = 7;
	parse_map["MODE"] = 8;
	parse_map["KICK"] = 9;
	parse_map["QUIT"] = 10;
	parse_map["INVITE"] = 11;
	parse_map["CAP"] = 12;
	parse_map["PART"] = 13;
}

void Server::init()
{
	this->srv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (srv_sock == -1)
		throw ("socket() error");
	this->setting_socket(this->srv_sock);
	this->serv_addr.sin_family = AF_INET;
	this->serv_addr.sin_addr.s_addr = INADDR_ANY;
	this->serv_addr.sin_port = htons(this->srv_port);
	if (bind(this->srv_sock, reinterpret_cast<sockaddr*>(&serv_addr), sizeof(serv_addr)) == -1)
		throw ("bind() error");
	if (listen(this->srv_sock, 5) == -1)
		throw("listen() error");
}

void Server::setting_socket(int srv_sock)
{
	int value = 1;
	if (setsockopt(srv_sock, SOL_SOCKET, SO_REUSEADDR, (const void *)&value, sizeof(value)) == -1)
		throw ("setsoctopt() error");
	if (fcntl(srv_sock, F_SETFL, O_NONBLOCK) == -1)
		throw ("fcntl() error");
}

void Server::make_event_window()
{
	fds[0].fd = this->srv_sock;
	fds[0].events = POLLIN | POLLERR | POLLHUP;
	for (int i = 1; i < MAXCLIENT + 1; ++i)
		fds[i].fd = -1;
}

void Server::execute()
{
	this->make_event_window();
	while (1)
	{
		int check = poll(fds, MAXCLIENT + 1, -1);
		// std::cout << "이벤트 발생" << std::endl;
		if (check == -1)
		{
			if (errno == 4)
				throw(1);
			throw("poll() error");
		}
		try
		{
			if (fds[0].revents & POLLIN)
			{
				// std::cout << "연결했음" << std::endl;
				accept_client();
			}
			if (fds[0].revents & POLLHUP || fds[0].revents & POLLERR)
				break ;
			for (int i = 1; i < MAXCLIENT + 1; i++)
			{
				// std::cout << i << ": " << fds[i].revents << " 상태보고" << std::endl;
				if (fds[i].fd == -1)
					continue ;
				if (fds[i].revents & POLLIN)
				{
					// std::cout << i << " 문자받음" << std::endl;
					message_receive(fds[i]);
				}
				if (fds[i].revents & POLLHUP || fds[i].revents & POLLERR)
				{
					// std::cout << i << ": 지워짐" << '\n';
					erase_clinet(fds[i]);
				}
			}
		}
		catch(const char *str)
		{
			std::cout << str << std::endl;
		}
	} 
}

void Server::accept_client()
{
	Client *cli = new Client;
	try
	{
		cli->set_socket(accept(this->srv_sock, reinterpret_cast<sockaddr*>(&cli->get_cil_addr()), &(cli->get_cli_size())));
		if (cli->get_socket() == -1)
			throw("accept() error");
		try
		{
			cli->setting_socket();
			this->find_vacant_fds().fd = cli->get_socket();
			this->insert_cli(cli);
		}
		catch(const char *str)
		{
			close(cli->get_socket());
			throw ;
		}
	}
	catch(const char *str)
	{
		std::cout << str << std::endl;
		delete cli;
	}
	// std::cout << "생성된 클라이언트 소켓 " << cli->get_socket() << std::endl;
}

struct pollfd &Server::find_vacant_fds()
{
	for(int i = 1; i < MAXCLIENT + 1; i++)
	{
		if (this->fds[i].fd == -1)
		{
			this->fds[i].events = POLLIN | POLLERR | POLLHUP;
			return (this->fds[i]);
		}
	}
	throw ("Connection limit exceeded");
	return (this->fds[0]);
}

void Server::erase_clinet(pollfd &fds)
{
	Client *cli;
	cli = this->find_client(fds.fd);
	this->cli_belong_channel_delete(cli);
	delete_cli(cli);
	delete cli;
	fds.fd = -1;
	fds.events = 0;
}

void Server::cli_belong_channel_delete(Client *cli)
{
	std::set<Channel *>::iterator it = this->ch_set.begin();
	std::set<Channel *>::iterator tmp = it;
	
	while(it != ch_set.end())
	{
		int cli_size_in_ch = 0;
		if ((*it)->find_cli_in_ch(cli) == NULL)
		{
			++it;
			continue;
		}
		cli_size_in_ch = (*it)->get_cli_lst_size();
		(*it)->delete_gm_cli_and_cli(cli);
		if (cli_size_in_ch == 1)
		{
			tmp = this->ch_set.erase(it);
			delete_ch(*it);
			delete (*it);
			it = tmp;
		}
	}
}

Client *Server::find_client(int fd)
{
	std::set<Client *>::iterator it = this->cli_set.begin();
	for (; it != cli_set.end(); it++)
	{
		if( (*it)->get_socket() == fd)
		{
			return ((*it));
		}
	}
	return (NULL);
}

void Server::message_receive(pollfd &fds)
{
	std::string message;
	Client *client;
	char buffer[1024];
	int message_size = 0;
	int idx;

	message_size = recv(fds.fd, buffer, 1024, 0);

	if (message_size != -1)
		buffer[message_size] = '\0';
	// std::cout << "받은 문자" << buffer << '\n';
	if (message_size == -1)
		throw (1);
	client = this->find_client(fds.fd);
	idx = client->append_submemory(buffer);
	if (idx == -1)
		return ;
	message = client->division_cmd(idx);
	// std::cout << "message: " << message << '\n';
	parse(message, client, *this);
}

void Server::insert_cli(Client *cli) { cli_set.insert(cli); }

void Server::delete_cli(Client *cli)
{
	std::set<Client *>::iterator it = this->cli_set.find(cli);
	if (it != this->cli_set.end())
		this->cli_set.erase(it);
}

void Server::insert_ch(Channel *ch) { ch_set.insert(ch); }

void Server::delete_ch(Channel *ch)
{
	std::set<Channel *>::iterator it = this->ch_set.find(ch);
	if (it != this->ch_set.end())
		this->ch_set.erase(it);
}

void Server::set_socket(int srv_sock){	this->srv_sock = srv_sock; }

int Server::get_socket() const{ 	return (this->srv_sock); }

int Server::get_cmd(std::string s) {	return (this->parse_map[s]); }

char* Server::get_passwd() const { return (this->passwd); }

Client* Server::find_cli_with_nick_name(std::string &nick_name)
{
	std::set<Client *>::iterator it = this->cli_set.begin();
	for (; it != cli_set.end(); it++)
	{
		if(!nick_name.compare((*it)->get_nick_name()))
		{
			return ((*it));
		}
	}
	return (NULL);
}

Channel* Server::find_ch_with_ch_name(std::string &channel_name)
{
    std::set<Channel *>::iterator it = this->ch_set.begin();
    for (; it != this->ch_set.end(); ++it)
    {
        if (!((*it)->get_ch_name().compare(channel_name)))
            return ((*it));
    }
    return (NULL);
}

void Server::password_err_message(Client *cli)
{
	ft_send(ERR_PASSWDMISMATCH, "your_client :Password incorrect", cli);
}

bool Server::check_pass_flag_cli_exit(Client *cli)
{
	if (cli->pass_flag == false)
	{
		/*
			Todo
			* 최적화할 예정
		*/
		for (int i = 1; MAXCLIENT + 1; ++i)
		{
			if (fds[i].fd == cli->get_socket())
			{
				delete_cli(cli);
				delete cli;
				close(fds[i].fd);
				fds[i].fd = -1;
				fds[i].events = 0;
				return (true);
			}
		}
	}
	return (false);
}

pollfd *Server::find_fds(int socket_num)
{
	for (int i = 1; i < MAXCLIENT + 1; i++)
	{
		if (this->fds[i].fd == socket_num)
			return (&(this->fds[i]));
	}
	return (NULL);
}

std::set<Channel *>::size_type Server::get_ch_set_size() const { return (this->ch_set.size()); }
std::set<Client *>::size_type Server::get_cli_set_size() const { return (this->cli_set.size()); }

Server::~Server()
{
	std::set<Channel *>::iterator ch_it = this->ch_set.begin();
    for (; ch_it != this->ch_set.end(); ++ch_it)
        delete (*ch_it);
	std::set<Client *>::iterator cli_it = this->cli_set.begin();
	for (; cli_it != this->cli_set.end(); ++cli_it)
        delete (*cli_it);
	close(this->srv_sock);
	std::cout << "종료" << std::endl;
}


std::set<Channel *> Server::find_cli_in_channels(Client *cli)
{
	std::set<Channel *> client_in_channels;
	std::set<Channel *>::iterator it = this->ch_set.begin();

	for (;it != ch_set.end(); ++it)
	{
		if ((*it)->find_cli_in_ch(cli) != NULL)
			client_in_channels.insert((*it));
	}
	return (client_in_channels);
}

