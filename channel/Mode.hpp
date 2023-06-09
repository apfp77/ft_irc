#ifndef MODE_HPP
#define MODE_HPP

#include <iostream>
#include <set>
#include "Client.hpp"

class Mode  {
	private:
 
		bool mode_invite;
		bool mode_topic;
		bool mode_key;
		bool mode_limit;
		std::string passwd;

		std::set<Client *> invite_cli_set;

		int cli_limit;

	public:
		void set_mode_invite(bool set);
		void set_mode_topic(bool set);
		void set_mode_key(bool set);
		void set_mode_limit(bool set);
		void set_cli_limit(int size);
		void set_passwd(std::string &passwd);


		bool get_mode_invite() const;
		bool get_mode_topic() const;
		bool get_mode_key() const;
		bool get_mode_limit() const;
		int	get_cli_limit() const;
		std::string get_passwd()const;

		void insert_invite_cli(Client *cli);
		void delete_invite_cli(Client *cli);

		bool is_allow_option(std::string &opt);
		void invite_cli_all_clean();

		std::string total_mode_string();

		bool check_cli_in_invite_cli_set(Client *cli);

		Mode();

		~Mode();
};

#endif
