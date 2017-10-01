#include "chatlog.hpp"

ChatLog::ChatLog()
{
    this->length = 0;
    this->iter_pos = 0;
}

ChatLog::ChatLog(std::vector<Messages> archive)
{
    for (int i = 0; i < archive.size(); i++)
    {
        this->log.push_back(archive[i]);
        this->length++;
    }
    
    this->iter_pos = 0;
}

ChatLog::~ChatLog()
{

}

void ChatLog::add(Messages msg)
{
    this->log.push_back(msg);
    this->length++;
}

std::vector<Messages> ChatLog::read()
{
    return this->log;
}

void ChatLog::clear()
{
    this->log.clear();
}

int ChatLog::get_length()
{
    return this->length;
}

Messages ChatLog::iter()
{
    if (iter_pos == this->length)
    {
        return this->log[iter_pos-1];
    }
    else
    {
        iter_pos++;

        return this->log[iter_pos-1];
    }
    
}

void ChatLog::iter_clear()
{
    iter_pos = 0;
}