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
	*/
	std::vector <std::string> recv_vector = split(recv, ' ');
	if (!recv_vector[0].compare("PING"))
	{
		
	}
	else if (!recv_vector[0].compare("TOPIC"))
	{

	}
	else if (!recv_vector[0].compare("MODE"))
	{

	}
	else if (!recv_vector[0].compare("KICK"))
	{

	}
	else if (!recv_vector[0].compare("PRIVMSG"))
	{

	}
	else
	{
		ft_send(ERR_NEEDMOREPARAMS, "Error: :Unknown command", cli);
	}
	(void)serv;
}