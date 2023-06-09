#include "ft_utils.hpp"

bool string_isalnum(std::string &s)
{
	if (s.length() == 0)
		return (false);
	for (int i = 0; s[i]; i++)
	{
		if (!std::isalnum(s[i]))
			return (false);
	}
	return (true);
}

void ft_send(std::string code, std::string s, Client *cli)
{
	std::string ret = "";
	if (code.compare(""))
		ret = code + " ";
	ret = ret + s + "\r\n";
	int check = send(cli->get_socket(), ret.c_str(), ret.length(), 0);
	if (check == -1)
	{ 
		std::cerr << "Failed to send data" << std::endl;
	}
}

std::vector<std::string> ft_split(std::string &str, std::string delimiter)
{
	std::vector<std::string> ret;
	std::string::size_type i = 0;
	std::string::size_type tmp_i = 0;
	std::string::size_type str_size = str.length();

	while (tmp_i != str_size)
	{
		i = tmp_i;
		i = str.find(delimiter, i);
		if (i == std::string::npos)
			break;
		ret.push_back(str.substr(tmp_i ,i - tmp_i));
		tmp_i = i + delimiter.length();
	}
	if (tmp_i != str_size)
		ret.push_back(str.substr(tmp_i ,str_size - tmp_i));
	return (ret);
}

int return_string_type(std::string &s)
{
	if (s[0] == '#')
		return (CHANNEL);
	return (USER);
}