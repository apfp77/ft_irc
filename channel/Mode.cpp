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

Mode::~Mode()
{

}
