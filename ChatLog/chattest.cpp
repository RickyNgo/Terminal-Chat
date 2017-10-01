#include "chatlog.hpp"
#include <iostream>

int main()
{
    Messages msg1("test1", "test1", "chatlog1");
    Messages msg2("test2", "test2","chatlog2");
    Messages msg3("test3", "test3","chatlog3");

    
    ChatLog log;
    log.add(msg1);
    log.add(msg2);
    log.add(msg3);

    std::cout << "Length " << log.get_length() << std::endl;

    std::vector<Messages> toBeRead = log.read();
    
    for (int i = 0; i < toBeRead.size(); i++)
    {
        std::cout << toBeRead[i].body() << std::endl;
    }
    return 0;
}