#ifndef CHATLOG_HPP
#define CHATLOG_HPP

#include "messages.hpp"
#include <vector>

class ChatLog
{
	public:
		ChatLog();
		ChatLog(std::vector<Messages> archive);
		~ChatLog();
		void add(Messages msg);
		std::vector<Messages> read();
		int get_length();
		void clear();
		Messages iter();
		void iter_clear();
		void delete_msg(int pos, std::string msg, std::string sender_id);

	private:
		std::vector<Messages> log;
		int length;
		int iter_pos;
};

#endif