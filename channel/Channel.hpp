#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include "Mode.hpp"
#include "Client.hpp"
#include <set>
#include <vector>

class Channel : public Mode {
	private:
		std::string ch_name;
		std::set<Client *> gm_lst;
		std::string passwd;

		std::string topic_message;
		std::set<Client *> cli_lst;

	public:
		Channel(std::string &ch_name, Client *cli);
		~Channel();
		std::string get_ch_name() const;
		
		void set_passwd(std::string &passwd);
		std::string get_passwd() const ;

		std::set<Client *>::size_type get_cli_lst_size() const;

		Client* find_cli_in_ch(Client *cli);
		Client* find_cli_in_gm_ch(Client *cli);
		Client* find_cli_in_ch_by_str(std::string &str);
		bool find_cli_in_gm_lst(Client *cli);
		Client* find_cli_in_gm_lst_by_str(std::string &str);

		void send_to_ch(std::string &message, Client *cli);
		void all_send_to_ch(std::string &message);

		void insert_cli(Client *cli);
		void insert_cli_gm(Client *cli);

		void delete_cli(Client *cli);
		void delete_cli_gm(Client *cli);
		void delete_gm_cli_and_cli(Client *cli);

		std::string get_topic() const;
		void set_topic(std::string new_message);

		std::vector<std::string> get_cli_names_in_ch();
};

#endif
