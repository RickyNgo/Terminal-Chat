#include "messages.hpp"


Messages::Messages()
{
	this->sender_id = "";
	this->recipient_id = ALL_CHAT;
	this->msg_length = 0;
	this->msg_body = "";
}

Messages::Messages(std::string sender_id, std::string msg_body)
{
	this->sender_id = sender_id;
	this->recipient_id = ALL_CHAT;
	this->msg_length = msg_body.length();
	this->msg_body = msg_body;
}

Messages::Messages(std::string sender_id, std::string recipient_id, std::string msg_body)
{
	this->sender_id = sender_id;
	this->recipient_id = recipient_id;
	this->msg_length = msg_body.length();
	this->msg_body = msg_body;
}

Messages::~Messages()
{
}

int Messages::length()
{
	return this->msg_length;
}

const char* Messages::body()
{
	return this->msg_body.c_str();
}
