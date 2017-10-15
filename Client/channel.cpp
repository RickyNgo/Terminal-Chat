
#include <boost/asio.hpp>
#include <cstdlib>
#include <string>

using boost::asio::ip::tcp;

#include "channel.hpp"

Channel::Channel(std::string name, int id):
    channel_name(name), channel_id(id), role(NO_ROLE), type(NO_TYPE)
    {}

Channel::~Channel(){   
    if(channel_socket){
        //close socket first
        free(channel_socket);
    }
}

void Channel::set_channel_name(std::string name){
    channel_name = name;
}

void Channel::set_channel_id(int id){
    channel_id = id;
}

void Channel::set_channel_socket(tcp::socket* socket){
    channel_socket = socket;
}

std::string Channel::get_channel_name(){
    return channel_name;
}

int Channel::get_channel_id(){
    return channel_id;
}

tcp::socket* Channel::get_channel_socket(){
    return channel_socket;
}