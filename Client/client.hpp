#ifndef client_hpp
#define client_hpp

#include <boost/asio.hpp>
#include <cstdlib>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <deque>
#include <thread>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include "channel.hpp"
#include "messages.hpp"


using boost::asio::ip::tcp;

#define HEADER_LEN 30

class Client :
public boost::enable_shared_from_this<Client>
{
private:
	boost::asio::io_service& ios;
	tcp::socket main_socket_;

	std::string user_alias_;
	int user_id_;
	std::map<int, Channel*> client_channels_;
	std::map<int,std::string> friend_list_;
    int current_channel_;
    
    uint8_t command_;
    uint8_t	body_length_;
    
    //need a counter so i don't get duplicate channel ids

private:
    void do_connect_(tcp::resolver::iterator);
	
    void do_read_header();
    void do_read_body();
	
    void on_read_header( boost::system::error_code, std::size_t);
    void on_read_body( boost::system::error_code, std::size_t);
	
    void do_write_header(Messages);
    void do_write_body(Messages);
	
    void on_write_header(boost::system::error_code, std::size_t, Messages);
    void on_write_body(boost::system::error_code, std::size_t);
    
    Messages read_msg;

    char read_buffer_[512];

public:
	Client(boost::asio::io_service&, tcp::resolver::iterator);
    
    void choose_alias();

    void send(Messages);
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
	void set_friend_list(std::map<int, std::string>);
    void set_current_channel(Channel*);

	//add reading/writing functions, integrate with other classes

    void add_friend(int, std::string);
	void add_channel(Channel*, int);
	
	void remove_channel(int);

	void setup_client(std::string, int);
    
    //verifies that argument meets syntax structure
    //set to between 5 and 25 characters, alphanumeric only
    bool syntax_valid_alias(std::string);
    
    //prompts for input and captures string until newline is entered
    //std::string get_input();
    
    //clean input of leading and trailing whitespace
    //returns 1 if string starts with '/' or 0 if not
    int clean_input(std::string);
    
    //strips first word off string
    std::string get_command(std::string);
    
    //compares argument against list of commands
    //returns command # if found or -1 if not found
    int find_command(std::string, std::string);
    
    // -2 if command is not allowed in current room
    // -3 if user does not have correct permissions for command
    int check_channel_permissions(int);
    
    // 0 if general command, 1 if channel only, 2 if mod/admin only, 3 if admin only
    int command_cat(int);
    
    //if command is a client side command, this function calls the appropriate action
    void client_command(int, std::string);
    
    //if command is a server side command, checks permissions for desired command and either packages and sends message if allowed
    //or displays error if not allowed
    void server_command(int, std::string);

};

#endif
