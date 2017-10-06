#ifndef MESSAGES_HPP
#define MESSAGES_HPP

#include <string>
#include <cstring>
#include <boost/asio.hpp>

enum Commands {MSG = 0, EXIT = 1, KICK_MOD = 2, KICK_USER = 3, ADD_MOD = 4, MESSAGE_DELETE = 5, SHOW = 6, JOIN = 16, CREATE = 17,
WHISPER = 18, INVITE = 19, FRIEND_LIST = 20, BLOCK = 21, ADD_FRIEND =22, ADD_CHANNEL = 23, ONLINE = 24, HELP = 25};

/*
//Channel Only
EXIT
KICK_MOD
KICK_USER
ADD_MOD
MESSAGE_DELETE
SHOW

//Server Only
JOIN
CREATE
WHISPER
INVITE
FRIEND_LIST
BLOCK
ADD_FRIEND
ADD_CHANNEL
ONLINE

//Client Only
HELP
*/

// Maximum length of a message body
const int MAX_MSG_LENGTH = 280;

// Recipient_id will default to this unless the sender whispers another client
const std::string ALL_CHAT = "ALL_CHAT";
 
class Messages
{
	public:
		// Empty Message
		Messages();

		// Client must parse info before passing into Message constructor
		// Message meant for the whole channel
		Messages(std::string sender_id, std::string msg_body, std::string host, int port, Commands cmd);

		// Message meant for a single person
		Messages(std::string sender_id, std::string recipient_id, std::string msg_body, std::string host, int port, Commands cmd);

		~Messages();

		void send();

		// Returns the sender_id
		std::string sender();
		
		// Returns the recipeint_id
		std::string recipient();
		
		// Returns the body length
		int length();
		
		// Returns the msg body
		std::string msg();
	
		// Converts the C++ string to a C string for socket transfer
		const char* body();

	private:
		std::string sender_id;
		std::string recipient_id;
		std::string msg_body;
		std::string host;
		time_t time;
		int msg_length;
		int port;
		unsigned int command;
};

#endif
