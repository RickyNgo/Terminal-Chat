#include "messages.hpp"
#include <ctime>
#include <iostream>

Messages::Messages()
{
	this->sender_id = "";
	this->msg_body = "";
	this->timestamp = time(NULL);
	this->command = 0;
	this->body_length = 0;
	this->header = "";
}

Messages::Messages(std::string sender, std::string msg_body, time_t t, int command)
{
	this->sender_id = sender;
	this->msg_body = msg_body;
	this->timestamp = t;
	this->command = command;
	this->body_length = msg_body.length();
	
	if (this->command < 10)
	{
		this->header.append("0");
		this->header.append(std::to_string(command));
		//this->header.append("-");
	}
	else
	{
		this->header.append(std::to_string(command));
		//this->header.append("-");
	}

	if (this->body_length < 10)
	{
		this->header.append("00");
		this->header.append(std::to_string(this->body_length));
		//this->header.append("-");
	}
	else if (this->body_length < 100)
	{
		this->header.append("0");
		this->header.append(std::to_string(this->body_length));
		//this->header.append("-");
	}
	else
	{
		this->header.append(std::to_string(this->body_length));
		//this->header.append("-");
	}

	this->header.append(std::to_string(this->timestamp));
	//this->header.append("-");
	this->header.append(sender_id);
	

	for (int i = 0; i < (MAX_ALIAS_LENGTH - this->sender_id.length()); i++)
	{
		this->header.append("0");
	}

}

// Takes a C-string and parses it into its respective segments.
/*
Messages::Messages(const char * server_msg)
{
	char msg[512];

	strcpy(msg, server_msg);

	std::vector<std::string> msg_contents;

	char *token = strtok(msg, "|");
	
	//std::cout << "Before tokenizing" << std::endl;
	while (token != NULL)
	{
		std::string conv(token);
		std::cout << conv << std::endl;
		msg_contents.push_back(token);
		token = strtok(NULL, "|");
	}
	
	//std::cout << "After tokenizing" << std::endl;	
	
	
	//std::cout << msg_contents[0] << std::endl;
	//std::cout << msg_contents[1] << std::endl;
	//std::cout << msg_contents[2] << std::endl;
	//std::cout << msg_contents[3] << std::endl;
	
	this->command = std::stoi(msg_contents[0]);
	this->timestamp = std::stoi(msg_contents[1]);
	this->sender_id = msg_contents[2];
	this->msg_body = msg_contents[3];
	
}
*/

Messages::~Messages()
{

}

// Takes the Message contents and concatenates them into a C-string for sending over sockets.
/*
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
*/
// length+command+time+sender+body



std::string Messages::get_sender()
{
	return this->sender_id;
}

std::string& Messages::get_body()
{
	return this->msg_body;
}

time_t Messages::get_time()
{
	return this->timestamp;
}

int Messages::get_command()
{
	return this->command;
}

int& Messages::get_length()
{
	return this->body_length;
}

std::string& Messages::get_header()
{
	return this->header;
}

void Messages::clear()
{
	this->sender_id = "";
	this->msg_body = "";
	this->header = "";
	this->timestamp = 0;
	this->command = 0;
	this->body_length = 0;
}

void Messages::parse_header()
{
	this->command = std::stoi(this->header.substr(0, 2));
	//std::cout << this->command << std::endl;

	this->body_length = std::stoi(this->header.substr(2, 3));
	//std::cout << this->body_length << std::endl;

	this->timestamp = std::stoi(this->header.substr(5, 15));
	//std::cout << this->timestamp << std::endl;

	this->sender_id = this->header.substr(15, 30);
	//std::cout << this->sender_id << std::endl;

	for (int i = 0; i < this->sender_id.length(); i++)
	{
		if (this->sender_id[i] == '0')
		{
			this->sender_id.erase(i, std::string::npos);
			return;
		}
	}
}