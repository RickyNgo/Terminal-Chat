#include <string>

// Maximum length of a message body
const int MAX_MESSAGE_LENGTH = 280;

// Recipient_id will default to this unless the sender whispers another client
const std::string ALL_CHAT = "ALL_CHAT";
 
class Messages
{
	public:
		int message_length;
		std::string message_body;
	private:
		std::string sender_id;
		std::string recipient_id;
};
