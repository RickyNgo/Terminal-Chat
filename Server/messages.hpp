#ifndef MESSAGES_HPP
#define MESSAGES_HPP

#include <string>
#include <cstring>
#include <vector>


enum Commands {MSG = 0, EXIT = 1, KICK_MOD = 2, KICK_USER = 3, ADD_MOD = 4, MESSAGE_DELETE = 5, SHOW = 6, JOIN = 16, CREATE = 17,
WHISPER = 18, INVITE = 19, FRIEND_LIST = 20, BLOCK = 21, ADD_FRIEND = 22, CREATE_CHANNEL = 23, ONLINE = 24, HELP = 25, LOGIN = 26,
INVITE_YES = 27, INVITE_NO = 28, INVITE_REQUEST = 29, CHANNEL_UPDATE = 30, CHANNEL_CLOSE = 31, LEAVE = 32};


// Maximum length of a message body
const int MAX_MSG_LENGTH = 280;
 
class Messages
{
	public:
		// Empty Message
		Messages();

		// Client must parse info before passing into Message constructor
		Messages(std::string sender, std::string msg_body, time_t t, int command);

		// Parses strings from the server
		Messages(char server_msg[]);

		~Messages();
		
		// Converts the C++ string to a C string for socket transfer
		std::string encode();


		std::string get_sender();
		std::string get_body();
		int get_time();
		int get_command();

	private:
		std::string sender_id;
		std::string msg_body;
		time_t timestamp;
		int command;
};

#endif