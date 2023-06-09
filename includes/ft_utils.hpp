#ifndef FT_UTILS_HPP
#define FT_UTILS_HPP

#include <Client.hpp>
#include <string>
#include <vector>

#define STRING 1
#define USER 2
#define CHANNEL 3

void ft_send(std::string code, std::string s, Client *cli);
std::vector <std::string> ft_split(std::string &str, std::string delimiter);
bool string_isalnum(std::string &s);
int return_string_type(std::string &s);

#endif