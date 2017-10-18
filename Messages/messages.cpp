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

// Takes a C-string and parses it into its respective segments.
Messages::Messages(char *server_msg)
{
	std::string msg(server_msg);

	std::string delimiter = "|";

	std::vector<std::string> msg_contents;

	int pos = 0;
	
	while ((pos = msg.find(delimiter)) != std::string::npos)
	{
		msg_contents.push_back(msg.substr(0, pos));
		s.erase(0, pos + delimiter.length());
	}

	this->command = std::stoi(msg_contents[1]);
	this->time = std::stoi(msg_contents[2]);
	this->sender_id = std::stoi(msg_contents[3]);
	this->msg_body = std::stoi(msg_contents[4]);
}

Messages::~Messages()
{

}

// Takes the Message contents and concatenates them into a C-string for sending over sockets.
const char* Messages::encode()
{
	std::string cmd_conv = std::to_string(this->command);
	std::string t_conv = std::to_string(this->time);

	int length = cmd_conv.length() + t_conv.length() + this->sender_id.length() + this->msg_body.length() + 4;
	std::string length_conv = std::to_string(length);

	std::string to_send = length_conv + "|" + cmd_conv + "|" + t_conv + "|" + this->sender_id + "|" + this->msg_body;

	return to_send.c_str();
}

// length+command+time+sender+body