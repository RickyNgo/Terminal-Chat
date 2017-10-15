#ifndef client_hpp
#define client_hpp

#include <boost/asio.hpp>
#include <cstdlib>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <thread>
#include "channel.hpp"

using boost::asio::ip::tcp;

class Client{
private:
	boost::asio::io_service& ios;
	tcp::socket main_socket_;
	//chat_message read_msg_; //	CHANGE TO RICKY'S CLASS NAME
	//chat_message_queue write_msgs_; // "

	std::string user_alias_;
	int user_id_;
	std::map<int,std::string> friend_list_;
    std::vector<Channel*> client_channels_;
    int current_channel_;

private:
    void do_connect_(tcp::resolver::iterator);
	void do_read_header_();
	void do_read_body_();
	void do_write_();

public:
	Client(boost::asio::io_service&, tcp::resolver::iterator);

	void write();
	void close();
	std::string show_help();

	std::string get_user_alias();
	int get_user_id();
	std::vector<std::string> get_friend_list();
    std::vector<Channel*> get_channels();
    tcp::socket* get_main_socket();

	void set_user_alias(std::string);	
	void set_user_id(int);
	void set_friend_list(std::map<int, std::string>); //change this to whatever the server sends the friend list as
	void set_channel_list(std::vector<Channel*>);

	//add reading/writing functions, integrate with other classes

    void add_friend(int, std::string);
	void add_channel(Channel*);

	void remove_channel(Channel*);

	void setup_client(std::string, int);

};

#endif
