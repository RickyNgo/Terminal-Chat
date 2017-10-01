#include "messages.hpp"
#include <vector>

class ChatLog
{
	public:
		add(Messages msg);
		read();
		clear();
		
	private:
		std::vector<Messages> log;
		int length;
};
