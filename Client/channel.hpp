
#ifndef channel_hpp
#define channel_hpp

#include <boost/asio.hpp>
#include <cstdlib>
#include <string>

using boost::asio::ip::tcp;

enum ChannelRole {ADMIN, MOD, GUEST, OWNER, NO_ROLE};
enum ChannelType {PRIVATE, GROUP, NO_TYPE};

class Channel{
private:
    std::string channel_name;
    int channel_id;
    tcp::socket* channel_socket;
    ChannelRole role;
    ChannelType type;
    
public:
    Channel(std::string, int);
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
