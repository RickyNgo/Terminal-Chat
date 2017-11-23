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
    int connection_port_;
    Channel* current_channel_;
	std::map<int, Channel*> client_channels_;
	std::map<int,std::string> friend_list_;
   
    uint8_t command_;
    uint8_t	body_length_;

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
	Client(boost::asio::io_service&, tcp::resolver::iterator, int);
    
    void choose_alias();
    void close();

    void send(Messages);

	std::string get_user_alias();
	int get_user_id();
	std::vector<std::string> get_friend_list();
    int get_channel_list_size();
    tcp::socket* get_main_socket();
    int get_current_channel_id();
    Channel* get_current_channel();

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

    // MSG, - don't need
    // KICK_USER, ** can package with leave s
    // ADD_MOD, ** s
    // JOIN_CHANNEL, ** cs
    // CREATE_CHANNEL, ** s
    // WHISPER, ** cs can package with create/join
    // INVITE_USER, ** c 
    // ONLINE, ** cs
    // LOGIN, ** cs
    // INVITE_YES, ** c 
    // INVITE_NO, ** c
    // CHANNEL_UPDATE, ** cs 
    // CHANNEL_CLOSE, ** cs
    // LEAVE ** cs
    // exit ** can package with leave c

    std::string show_help();

    //share socket by reference
    //use main socket for general commands, not channel specific commands
    //only time to read for general commands is not in channel

    // client side, FROM client TO server
    void exit_enki(); //***
    void join(std::string); //*** send channel name and port #, and then if ok, listen
    void add_mod(std::string);
    void create_channel(std::string); //***
    void whisper(std::string);
    void invite_user(std::string);
    void kick(std::string);
    void online();
    void invite_yes();
    void invite_no();
    void channel_update(std::string);
    void channel_close();
    void leave();

    // server side, FROM server TO client
    void s_kick_user();
    void s_add_mod();
    void s_join_channel(std::string);
    void s_create_channel(); // if ok, get port #, send port # back then listen
    void s_whisper();
    void s_online(std::vector<std::string>);
    void s_channel_update();
    void s_channel_close();
    void s_leave();

};

#endif
