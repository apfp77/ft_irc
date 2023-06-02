#ifndef FT_UTILS_HPP
#define FT_UTILS_HPP

#include <Client.hpp>
#include <string>
#include <vector>

void ft_send(std::string code, std::string s, Client *cli, bool err);
std::vector <std::string> ft_split(std::string &str, std::string delimiter);

#endif