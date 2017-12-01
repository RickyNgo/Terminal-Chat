#ifndef MESSAGES_HPP
#define MESSAGES_HPP

#include <string>
#include <cstring>
#include <vector>

enum Commands {MSG, KICK_USER, ADD_MOD, JOIN, WHISPER, INVITE_USER, ONLINE, LOGIN, 
				INVITE_YES, INVITE_NO, CHANNEL_UPDATE, CHANNEL_CLOSE, LEAVE};


// Maximum length of a message body
const int MAX_MSG_LENGTH = 280;

// Max length of message header
const int MAX_HEADER_LENGTH = 30;

// Max length of alias
const int MAX_ALIAS_LENGTH = 15;
class Messages
{
	public:
		// Empty Message
		Messages();

		// Client must parse info before passing into Message constructor
		Messages(std::string sender, std::string msg_body, time_t t, Commands command);

		~Messages();
		
		std::string get_sender();
		
		std::string& get_body();

		std::string& get_header();

		time_t get_time();
		
		int get_command();
		
		int& get_length();
		
		void clear();
		
		void parse_header();

	private:
		std::string sender_id;
		std::string msg_body;
		std::string header;
		time_t timestamp;
		int command;
		int body_length;
};

#endif
