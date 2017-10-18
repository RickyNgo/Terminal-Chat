#include "messages.hpp"

Messages::Messages()
{
	this->sender_id = "";
	this->msg_body = "";
	this->time = time(NULL);
	this->command = 0;
}

Messages::Messages(std::string sender, std::string msg_body, time_t t, int command)
{
	this->sender_id = sender;
	this->msg_body = msg_body;
	this->time = t;
	this->command = command;
}

Messages::Messages(char *server_msg)
{
	
}

Messages::~Messages()
{

}


const char* Messages::encode()
{
	std::string cmd_conv = std::to_string(this->command);
	std::string t_conv = std::to_string(this->time);
}

// length+command+time+sender+body