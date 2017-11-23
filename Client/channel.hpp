
#ifndef channel_hpp
#define channel_hpp

#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <cstdlib>
#include <string>
#include "messages.hpp"

using boost::asio::ip::tcp;

enum ChannelRole {ADMIN, MOD, GUEST, OWNER, NO_ROLE};
enum ChannelType {PRIVATE, GROUP, NO_TYPE};

class Channel:
public boost::enable_shared_from_this<Channel>
{
private:
    std::string channel_name;
    int channel_id;
    tcp::socket& channel_socket_;
    ChannelRole role;
    ChannelType type;

    uint8_t command_;
    uint8_t body_length_;

private:
    void do_connect_(tcp::resolver::iterator);

    void do_read_header();
    void do_read_body();
    
    void on_read_header( boost::system::error_code, std::size_t);
    void on_read_body( boost::system::error_code, std::size_t);

    Messages read_msg;

    char read_buffer_[512];
    
public:
    Channel(std::string, int, tcp::socket&);
    ~Channel();
    
    void set_channel_name(std::string);
    void set_channel_id(int);
    void set_channel_socket(tcp::socket*);
    void set_channel_type(ChannelType);
    void set_channel_role(ChannelRole);
    
    std::string get_channel_name();
    int get_channel_id();
    tcp::socket* get_channel_socket();
    ChannelType get_channel_type();
    ChannelRole get_channel_role();
    
};

#endif
