#ifndef MESSAGES_HPP
#define MESSAGES_HPP

#include <string>
#include <cstring>

// Maximum length of a message body
const int MAX_MSG_LENGTH = 280;

// Recipient_id will default to this unless the sender whispers another client
const std::string ALL_CHAT = "ALL_CHAT";
 
class Messages
{
	public:
		// Empty Message
		Messages();

		// Message meant for the whole channel
		Messages(std::string sender_id, std::string msg_body);

		// Message meant for a single person
		Messages(std::string sender_id, std::string recipient_id, std::string msg_body);
		~Messages();

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
		int msg_length;
		std::string msg_body;
};

#endif
