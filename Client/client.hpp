#ifndef client_hpp
#define client_hpp

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <cstdlib>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <thread>

#include "channel.hpp"
#include "aux_functions.hpp"
#include "messages.hpp"

using boost::asio::ip::tcp;

class Client: public boost::enable_shared_from_this<Client>{
private:
	boost::asio::io_service& ios;
	tcp::socket main_socket_;

	std::string user_alias_;
	int user_id_;
	std::map<int, Channel*> client_channels_;
	std::map<int,std::string> friend_list_;
    int current_channel_;
    //need a counter so i don't get duplicate channel ids

private:
    void do_connect_(tcp::resolver::iterator);
	void do_read_header_();
	void do_read_body_();
    void do_write_(std::string);

public:
	Client(boost::asio::io_service&, tcp::resolver::iterator);
    
    void choose_alias();

    void write(std::string);
    void handler(const boost::system::error_code&, std::size_t);
	void close();
	std::string show_help();

	std::string get_user_alias();
	int get_user_id();
	std::vector<std::string> get_friend_list();
    int get_channel_list_size();
    tcp::socket* get_main_socket();
    int get_current_channel_id();
    Channel* get_channel_from_id(int);

	void set_user_alias(std::string);	
	void set_user_id(int);
	void set_friend_list(std::map<int, std::string>); //change this to whatever the server sends the friend list as
	void set_current_channel(Channel*);

	//add reading/writing functions, integrate with other classes

    void add_friend(int, std::string);
	void add_channel(Channel*, int);
	
	void remove_channel(int);

	void setup_client(std::string, int);

};

#endif
