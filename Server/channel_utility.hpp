#ifndef CHANNEL_UTILITY_HPP
#define CHANNEL_UTILITY_HPP

#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>

//#include "session.hpp"
#include "messages.hpp"

class Session;

class ChannelUtility
{
public:
    void send_msg(boost::shared_ptr<Session> recipient, Messages msg);
private:
    
};









#endif 

