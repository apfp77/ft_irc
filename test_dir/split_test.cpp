#include <iostream>
#include <vector>
#include <string>

std::vector <std::string> ft_split(std::string &str, std::string delimiter)
{
	std::vector <std::string>ret;
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

int main(int argc, char const *argv[])
{
	std::string s = "PASS 4242\r\nNICK test7788\r\nUSER user 192.168.0.1 :test7788\r\n";
	std::vector<std::string> recv_vector = ft_split(s, "\r\n");

	
	std::vector <std::vector<std::string> > a;
	for (std::vector<std::string>::size_type i = 0; i < recv_vector.size(); i++)
		a.push_back(ft_split(recv_vector[i], " "));
	for (std::vector<std::vector<std::string> >::size_type i = 0; i < a.size(); i++)
	{
		// std::cout << a[i].size() << '\n';
		for (std::vector<std::string>::size_type j = 0; j < a[i].size(); j++)
		{
			std::cout << a[i][j];
		}	
	}

	return 0;
}
