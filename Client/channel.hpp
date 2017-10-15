

#ifndef channel_hpp
#define channel_hpp

#include <boost/asio.hpp>
#include <cstdlib>
#include <string>

using boost::asio::ip::tcp;

enum ChannelRole {admin, mod, guest, owner, none};
enum ChannelType {private, group, none};

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
    
    std::string get_channel_name();
    int get_channel_id();
    tcp::socket* get_channel_socket();
    
};

#endif
