
#ifndef channel_hpp
#define channel_hpp

#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <cstdlib>
#include <string>
#include "../Messages/messages.hpp"

using boost::asio::ip::tcp;

enum ChannelRole {ADMIN, MOD, GUEST, OWNER, NO_ROLE};
enum ChannelType {PRIVATE, GROUP, NO_TYPE};

class Channel:
public boost::enable_shared_from_this<Channel>
{
private:
    std::string channel_name;
    int channel_id;
    int port;
    boost::shared_ptr<tcp::socket> channel_socket_;
    ChannelRole role;
    ChannelType type;
tcp::acceptor acceptor;

    uint8_t command_;
    uint8_t body_length_;

private:
    void do_connect_(tcp::resolver::iterator);

    void do_read_header();
    void do_read_body();
    
    void on_read_header( boost::system::error_code, std::size_t);
    void on_read_body( boost::system::error_code, std::size_t);
	
    void do_write_header();
    void do_read_body();
    void on_read_header(boost::system::error_code, std::size_t);
    void on_read_body( boost::system::error_code, std::size_t);

    Messages read_msg;
    void accept_handler(const boost::system::error_code&);
    char read_buffer_[512];
    
public:
    Channel(std::string, int, boost::asio::io_service&, int);
    ~Channel();
    void start();    
    void set_channel_name(std::string);
    void set_channel_id(int);
    void set_channel_socket(tcp::socket*);
    void set_channel_type(ChannelType);
    void set_channel_role(ChannelRole);
    
    std::string get_channel_name();
    int get_channel_id();
    boost::shared_ptr<tcp::socket> get_channel_socket();
    ChannelType get_channel_type();
    ChannelRole get_channel_role();
    
};

#endif
