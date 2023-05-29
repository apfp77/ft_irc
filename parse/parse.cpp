#include "parse.hpp"
#include "Numerics.hpp"

std::vector <std::string> split(std::string &s, char c)
{
	std::vector <std::string>ret;
	int i = 0;
	std::string tmp = "";

	while (s[i])
	{
		if (s[i] == c && tmp.compare(""))
		{
			ret.push_back(tmp);
			tmp = "";
		}
		else if (s[i] != c)
			tmp += s[i];
		i++;
	}
	if (tmp.compare(""))
			ret.push_back(tmp);
	return (ret);
}

void ft_send(std::string code, std::string s, Client *cli)
{
	std::string ret = "";
	ret += code;
	ret += s;
	int check = send(cli->get_socket(), ret.c_str(), ret.length(), 0);
	if (check == -1) { 
		std::cerr << "Failed to send data" << std::endl;
		/*
			클라이언트 클래스 해제
		*/
	}
}

void ft_pass(std::vector<std::string> &recv_vector, Client *cli, Server &serv)
{
	if (cli->pass_flag)
	{

	}
	if (recv_vector[1].compare(serv.get_passwd()))
	{

	}
	(void)recv_vector;
	(void)cli;
	(void)serv;
}

void ft_ping(std::vector<std::string> &recv_vector, Client *cli, Server &serv)
{
	(void)recv_vector;
	(void)cli;
	(void)serv;
}

void ft_nick(std::vector<std::string> &recv_vector, Client *cli, Server &serv)
{
	(void)recv_vector;
	(void)cli;
	(void)serv;
}

void ft_name(std::vector<std::string> &recv_vector, Client *cli, Server &serv)
{
	(void)recv_vector;
	(void)cli;
	(void)serv;
}

void ft_privmsg(std::vector<std::string> &recv_vector, Client *cli, Server &serv)
{
	(void)recv_vector;
	(void)cli;
	(void)serv;
}

void ft_topic(std::vector<std::string> &recv_vector, Client *cli, Server &serv)
{
	(void)recv_vector;
	(void)cli;
	(void)serv;
}

void ft_join(std::vector<std::string> &recv_vector, Client *cli, Server &serv)
{
	(void)recv_vector;
	(void)cli;
	(void)serv;
}

void ft_mode(std::vector<std::string> &recv_vector, Client *cli, Server &serv)
{
	(void)recv_vector;
	(void)cli;
	(void)serv;
}

void ft_kick(std::vector<std::string> &recv_vector, Client *cli, Server &serv)
{
	(void)recv_vector;
	(void)cli;
	(void)serv;
}

void parse(std::string recv, Client *cli, Server &serv)
{
	/*
		recv 파싱
		1. ping                                                                                                                                         
		2. topic
		3. mode
		4. kick
		5. PRIVMSG
		6. 그 외는 에러

	비밀번호 먼저 체크 후 접속 남은 명령어 수행
	PING, TOPIC, JOIN, MODE, PRIVMSG, KICK, NICK
	*/
	std::vector <std::string> recv_vector = split(recv, ' ');
	switch(serv.get_cmd(recv_vector[0].c_str()))
	{
		case PING:
			ft_ping(recv_vector, cli, serv);
			break;
		case PASS:
			ft_pass(recv_vector, cli, serv);
			break;
		case NICK:
			ft_nick(recv_vector, cli, serv);
			break;
		case NAME:
			ft_name(recv_vector, cli, serv);
			break;
		case PRIVMSG:
			ft_privmsg(recv_vector, cli, serv);
			break;
		case TOPIC:
			ft_topic(recv_vector, cli, serv);
			break;
		case JOIN:
			ft_join(recv_vector, cli, serv);
			break;
		case MODE:
			ft_mode(recv_vector, cli, serv);
			break;
		case KICK:
			ft_kick(recv_vector, cli, serv);
			break;
		default:
			ft_send(ERR_NOTREGISTERED, ":You have not registered", cli);
			break;
	}
}