#include "messages.hpp"


Messages::Messages()
{
	
	this->sender_id = "";
	this->recipient_id = ALL_CHAT;
	this->msg_length = 0;
	this->msg_body = "";
	this->host = "";
	this->time = std::time(NULL);
	this->port = 0;
	this->command = 0;
}

Messages::Messages(std::string sender_id, std::string msg_body, std::string host, int port, Commands cmd)
{
	this->sender_id = sender_id;
	this->recipient_id = ALL_CHAT;
	this->msg_length = msg_body.length();
	
	if (msg_body.length() <= MAX_MSG_LENGTH)
	{
		this->msg_body = msg_body;
	}
	else
	{
		this->msg_body = msg_body.substr(0, 279);
	}

	this->time = std::time(NULL);
	this->port = port;
	this->host = host;
	this->command = 1 << cmd;
}

Messages::Messages(std::string sender_id, std::string recipient_id, std::string msg_body)
{
	this->sender_id = sender_id;
	this->recipient_id = recipient_id;
	this->msg_length = msg_body.length();
	
	if (msg_body.length() <= MAX_MSG_LENGTH)
	{
		this->msg_body = msg_body;
	}
	else
	{
		this->msg_body = msg_body.substr(0, 279);
	}

	this->time = time(this->time);
	this->port = port;
	this->host = host;
	this->command = 1 << cmd;
}


void Messages::send()
{
	std::string msg;
	std::tm *tm = std::localtime(this->time);

	// "|" are delimiters and "@" is the end token
	msg = "|" + std::to_string(tm->tm_hour) + ":" + std::to_string(tm->tm_min) + ":" + std::to_string(tm->tm_sec);
	msg = "|" + std::to_string(this->port) + "|" + this->host + this->sender_id + "|" + this->recipient_id + "|" + this->msg_body + "@";
	msg.insert(0, std::to_string(command));

	
	boost::asio::io_service ios;
   	boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(this->host), this->port);
   	boost::asio::ip::tcp::socket socket(ios);
   	socket.connect(endpoint);
   	socket.write_some(msg.data(), msg.size(), error);
	socket.close();
}

Messages::~Messages()
{
}

std::string Messages::sender()
{
	return this->sender_id;
}

std::string Messages::recipient()
{
	return this->recipient_id;
}

int Messages::length()
{
	return this->msg_length;
}

const char* Messages::body()
{
	return this->msg_body.c_str();
}
