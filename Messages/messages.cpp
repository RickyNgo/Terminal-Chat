#include "messages.hpp"
#include <string>
#include <ctime>
#include <iostream>

Messages::Messages()
{
	this->sender_id = "";
	this->msg_body = "";
	this->timestamp = time(NULL);
	this->command = 0;
}

Messages::Messages(std::string sender, std::string msg_body, time_t t, int command)
{
	this->sender_id = sender;
	this->msg_body = msg_body;
	this->timestamp = t;
	this->command = command;
}

// Takes a C-string and parses it into its respective segments.
Messages::Messages(char server_msg[])
{
	char msg[512];

	strcpy(msg, server_msg);

	std::vector<std::string> msg_contents;

	char *token = strtok(msg, "|");
	
	std::cout << "Before tokenizing" << std::endl;
	while (token != NULL)
	{
		std::string conv(token);
		std::cout << conv << std::endl;
		msg_contents.push_back(token);
		token = strtok(NULL, "|");
	}
	
	std::cout << "After tokenizing" << std::endl;	
	
	
	std::cout << msg_contents[0] << std::endl;
	std::cout << msg_contents[1] << std::endl;
	std::cout << msg_contents[2] << std::endl;
	std::cout << msg_contents[3] << std::endl;
	
	

	
	this->command = std::stoi(msg_contents[0]);
	this->timestamp = std::stoi(msg_contents[1]);
	this->sender_id = msg_contents[2];
	this->msg_body = msg_contents[3];
	
}

Messages::~Messages()
{

}

// Takes the Message contents and concatenates them into a C-string for sending over sockets.
std::string Messages::encode()
{
	std::string cmd_conv = std::to_string(this->command);
	std::string t_conv = std::to_string(this->timestamp);

	int length = cmd_conv.length() + t_conv.length() + this->sender_id.length() + this->msg_body.length() + 4;
	std::string length_conv = std::to_string(length);

	std::string to_send = length_conv + "|" + cmd_conv + "|" + t_conv + "|" + this->sender_id + "|" + this->msg_body;

	//return to_send.c_str();

	return to_send;
}

// length+command+time+sender+body



std::string Messages::get_sender()
{
	return this->sender_id;
}

std::string Messages::get_body()
{
	return this->msg_body;
}

int Messages::get_time()
{
	return this->timestamp;
}

int Messages::get_command()
{
	return this->command;
}