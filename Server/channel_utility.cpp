#include "channel_utility.hpp"
#include "session.hpp"

void ChannelUtility::send_msg(boost::shared_ptr<Session> recipient, Messages msg)
{
    recipient->do_write_header();
}