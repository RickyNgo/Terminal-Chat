#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>
#include <cstdlib>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <thread>


#include "client.hpp"
#include "aux_functions.hpp"

#define MIN_ALIAS 5
#define MAX_ALIAS 15

using boost::asio::ip::tcp;

/***************************************
constructor
 ***************************************/
Client::Client(boost::asio::io_service& io_serv, tcp::resolver::iterator endpoint_iterator) : 
ios(io_serv),
main_socket_(io_serv),
user_alias_(""),
user_id_(-1),
current_channel_(0)
{
    do_connect_(endpoint_iterator);
}

/***************************************
 on_read_header
 
 read handler
 ***************************************/
void Client::on_read_header( boost::system::error_code ec, std::size_t bytes ) {
    if (!ec) {
        Messages msg;
        msg.get_header() = read_buffer_;
        
        msg.parse_header();
        body_length_ = msg.get_length();
        command_ = msg.get_command();
        
        std::cout << "header body length: " << msg.get_length() << "by member: " << body_length_ << std::endl;
        std::cout << "header command: " << msg.get_command() << "by member: " << command_ << std::endl;
        
        
        do_read_body();
    } else {
        std::cout << "Read error in on_read_header: " << ec << std::endl;
    }
}

/***************************************
 on_read_body
 
 read handler
 ***************************************/
void Client::on_read_body( boost::system::error_code ec, std::size_t bytes ) {
    if (!ec) {
        std::cout << "read_buffer_: " << this->read_buffer_ << std::endl;
        memset(read_buffer_, '\0', sizeof(char)*512);
        do_read_header();
    } else {
        std::cout << "Read error: " << ec << std::endl;
    }
}

/***************************************
 do_read_header
 
 ***************************************/
void Client::do_read_header() {
    main_socket_.async_receive(
                               boost::asio::buffer(
                                                   read_buffer_,
                                                   MAX_HEADER_LENGTH),
                               boost::bind(&Client::on_read_header,
                                           shared_from_this(),
                                           _1, _2 ));
    
    std::cerr << "called handler on_read_header" << std::endl;
    
}

/***************************************
 do_read_body
 
 
 ***************************************/
void Client::do_read_body() {
    main_socket_.async_receive(
                               boost::asio::buffer(
                                                   read_buffer_,
                                                   body_length_),
                               boost::bind(&Client::on_read_body,
                                           shared_from_this(),
                                           _1, _2 ));
    
    std::cerr << "called handler on_read_body" << std::endl;
    
}

/***************************************
 send
 
 ***************************************/
void Client::send(Messages msg) {
    this->do_write_header(msg);
}

/***************************************
 on_write_header
 
 write handler
 ***************************************/
void Client::on_write_header( boost::system::error_code error, size_t bytes, Messages msg) {
    
    if (!error){
        std::cout << "header bytes written: " << bytes << std::endl;
        do_write_body(msg);
    }
    else{
        std::cout << "header write not a success\n";
        std::cout << "category: " << error.category().name() << " code: " << error.value() << " message: " << error.message() << std::endl;
        std::cout << "bytes written: " << bytes << std::endl;
    }
}

/***************************************
 on_write_body
 
 write handler
 ***************************************/
void Client::on_write_body( boost::system::error_code error, size_t bytes) {
    
    if (!error){
        std::cout << "body bytes written: " << bytes<< std::endl;
    }
    else{
        std::cout << "write not a success\n";
        std::cout << "category: " << error.category().name() << " code: " << error.value() << " message: " << error.message() << std::endl;
        std::cout << "bytes written: " << bytes << std::endl;
    }
}

/***************************************
 do_write_header
 
 ***************************************/
void Client::do_write_header(Messages msg){
    // send to server
    std::cout << "do write header" <<std::endl;
    
    boost::asio::async_write(main_socket_,
                             boost::asio::buffer(msg.get_header(),
                                                 MAX_HEADER_LENGTH),
                             boost::bind( &Client::on_write_header,
                                         shared_from_this(),
                                         _1, _2, msg));
}

/***************************************
 do_write_body
 
 ***************************************/
void Client::do_write_body(Messages msg){
    // send to server
    std::cout << "do write body" << std::endl;
    
    boost::asio::async_write(main_socket_,
                             boost::asio::buffer(msg.get_body(),
                                                 msg.get_length()),
                             boost::bind( &Client::on_write_body,
                                         shared_from_this(),
                                         _1, _2));
}


/***************************************
 choose_alias
 
 ***************************************/
void Client::choose_alias(){
    std::string alias, alias_req, u_input, command;
    size_t request_length;
    time_t current_time;
    int valid_name = 0;
    
    do
    {
        std::cout << "Choose an alias: ";
        
        std::getline(std::cin, alias);
        
        valid_name = syntax_valid_alias(alias);
        
        if(!valid_name){
            std::cout << '\n' << "Not a valid alias. Must be between 5 and 15 alphanumeric characters" << std::endl;
        }
        
        else{
            //encode input
            time(&current_time);
            Messages alias_req(alias, alias, current_time, LOGIN);
            
            valid_name = true;

            do_write_header(alias_req);
//            
//            // update valid_name to true depending on response
//            if(alias_decode.get_body() == "OK"){
//                c.set_user_alias(alias);
//                std::cout << "Success! Here is the message body: " << alias_decode.get_body() << std::endl;
//                valid_name = true;
//            }
//            else{
//                std::cout << "Alias is taken. Choose another." << std::endl;
//            }
        }
        
    } while (!valid_name);
}


/***************************************
close
***************************************/
void Client::close(){
	ios.post([this](){ 
		main_socket_.close();
	});
}

/***************************************
show_help()
***************************************/
std::string Client::show_help(){
	std::string help = "Here is some basic help output";  //how to print this in correct place using ncurses?

	return help;
}

/***************************************
get_user_alias
***************************************/
std::string Client::get_user_alias(){return user_alias_;}

/***************************************
get_user_id
***************************************/
int Client::get_user_id(){return user_id_;}

/***************************************
get_friend_list
***************************************/
std::vector<std::string> Client::get_friend_list(){ //how to print this in correct place using ncurses?
	std::vector<std::string> f_list;
	std::map<int, std::string>::iterator it = friend_list_.begin();
 
	while (it != friend_list_.end())
	{
		f_list.push_back(it->second);
	}

	return f_list;

}

int Client::get_channel_list_size(){return client_channels_.size();}

tcp::socket* Client::get_main_socket(){return &main_socket_;}

int Client::get_current_channel_id(){return current_channel_;}

Channel* Client::get_channel_from_id(int id){
	return client_channels_[id];
}


/***************************************
set_user_alias
***************************************/
void Client::set_user_alias(std::string name){user_alias_ = name;} //perform validation outside of class

/***************************************
set_user_id
***************************************/
void Client::set_user_id(int id){user_id_ = id;}

/***************************************
set_friend_list
***************************************/
//change this to whatever the server sends the friend list as
void Client::set_friend_list(std::map<int, std::string> friends){friend_list_ = friends;} 

/***************************************
set current channel
***************************************/
void Client::set_current_channel(Channel* chan){current_channel_ = chan->get_channel_id();}

/***************************************
add_friend
***************************************/
void Client::add_friend(int id, std::string name){ //http://www.cplusplus.com/reference/map/map/insert/
    std::pair<std::map<int,std::string>::iterator,bool> ret;
    ret = friend_list_.insert(std::pair<int,std::string>(id,name));
	
	if (ret.second==false) {
		std::cout << "friend already added." << '\n'; // update this
	}
}

/***************************************
add_room
***************************************/
// replace parameter with ChatRoom
void Client::add_channel(Channel* chan, int id){
	std::pair<std::map<int,Channel*>::iterator,bool> ret;
	ret = client_channels_.insert (std::pair<int,Channel*>(id,chan));
	if (ret.second==false) {
		std::cout << "That channel already exists." << std::endl;
	}
}


/***************************************
remove_room
this is just removing the room from the vector, not actually closing the room. either do that in a separate function or do it here
***************************************/
void Client::remove_channel(int id){
	client_channels_.erase(id);
}

/***************************************
setup_client
this is going to need to partly moved to the constructor and partly broken out into other functions for db stuff. 
 either leave this function to corral them all, or do it somewhere else.
***************************************/
void Client::setup_client(std::string name, int id){
	set_user_alias(name);
	set_user_id(id);
}



/***************************************
 do_connect_
 
 custom connect for testing using handler
 ***************************************/
void Client::do_connect_(tcp::resolver::iterator endpoint_iterator){
	boost::asio::async_connect(main_socket_, endpoint_iterator,
        [this](boost::system::error_code ec, tcp::resolver::iterator){
            if (!ec){
            	do_read_header();
        	} else {
                std::cerr << "do_connect_ lamda: " << ec << std::endl;
            }
    });
}




//verifies that argument meets syntax structure
// right now, this is doing both alias and pw. could restructure for custom matching.
// set to between 5 and 15 characters, alphanumeric only
bool Client::syntax_valid_alias(std::string u_string){
    if(u_string.length() > MAX_ALIAS | u_string.length() < MIN_ALIAS){return false;}
    
    for(int i = 0; i<u_string.length(); i++){
        if(!isalnum(u_string[i])){return false;}
    }
    
    return true;
}

//prompts for input and captures string until newline is entered
//std::string get_input(){
//	std::string user_input;
//
//	std::cout << ">> ";
//	std::getline(std::cin, user_input, '\n');
//
//	return user_input;
//}

//clean input of leading and trailing whitespace
//returns 1 if string starts with '/' or 0 if not
int Client::clean_input(std::string input){
    boost::algorithm::trim(input);
    
    if(input[0] == '/'){return 1;}
    else {return 0;}
    
}

//strips first word off string
std::string Client::get_command(std::string input){
    std::string delim = " ";
    
    std::string token = input.substr(0, input.find(delim));
    
    find_command(token, input);
    
    return token;
}

//compares argument against list of commands
//returns command # if found or -1 if not found
//changed this a little to explicitly save the return value in order to call the
//nect function inside this one, yet keep same prototype

int Client::find_command(std::string command, std::string u_input){
    int command_num = -1;
    std::string command_list[] = {"/exit", "/show", "/join", "/create", "/whisper", "/online", "/help", "/invite", "/leave", "/kick", "/add_mod", "/channel_close"};
    
    boost::algorithm::to_lower(command);
    
    for(int i = 0; i < (sizeof(command_list)/sizeof(command_list[0])); i++){
        if(command == command_list[i]){
            command_num =  i;
            //place client function call here? with break?
        }
    }
    
    if(command_num != -1){
        if(command_num == 1 | command_num == 6){
            client_command(command_num, u_input);
        }
        
        else{
            server_command(command_num, u_input);
        }
    }
    
    return command_num;
}



// -1 if command is not allowed in current room for any level of permissions
// -2 if user does not have correct permissions for command in current room
int Client::check_channel_permissions(int command){
	//get ChannelType of current channel
	ChannelType ct = (get_channel_from_id(get_current_channel_id()))->get_channel_type();
	ChannelRole cr = (get_channel_from_id(get_current_channel_id()))->get_channel_role();


 	if(ct == NO_TYPE){
 		return -1;
 	}

 	else if(ct == GROUP){
 		switch(cr){
			case GUEST:
				if(command > 8){
					return -2;
				}
				break;

			case MOD:
				if(command > 9){
					return -2;
				}
				break;

			case NO_ROLE:
				std::cout << "Error: you must have a role in a group" << std::endl;
				return -2;
				break;

			default:
				break;
		}
 	}

 	else if(ct == PRIVATE){
		if(command == 4 | command == 7 | command == 9 | command == 10 | command == 11){
			return -1;
		}
 	}

	return 0;

}


//returns 0 if general command, 1 if "in channel only", 2 if mod/admin only, 3 if admin only
int Client::command_cat(int command){
    switch(command){
        case 0:
        case 2:
        case 3:
        case 4:
        case 5:
            return 0; //general
            break;
        case 7:
        case 8:
            return 1; //in channel only
            break;
        case 9:
            return 2; //mod or admin only
            break;
        case 10:
        case 11:
            return 3; //admin only
            break;
        default:
            return -1;
            break;
    }
}

//if command is a client side command, this function calls the appropriate action
void Client::client_command(int command, std::string message){
    if(command == 1){
        //show channel
        std::cout << "Here is your channel" << std::endl;
    }
    else if(command == 6){
        std::cout << "here is some help" << std::endl;
    }
    else{
        std::cout << "error. not a client command" << std::endl;
    }
}

//if command is a server side command, checks permissions for desired command and either packages and sends message if allowed
//or displays error if not allowed
void Client::server_command(int command, std::string message){
	int perm = check_channel_permissions(command);

	if(perm == -1){
		std::cout << "That command is not allowed here" << std::endl;
	}

	else if (perm == -2){
		std::cout << "You do not have the correct permissions for that action" << std::endl;
	}

	else{
		int command_type = command_cat(command);

		switch(command_type){
			case 0:
				//general command
				std::cout << "general command: making a Message and sending" << std::endl;
				//stick into Message and send
				break;
			case 1:
				//in channel only
				if(get_channel_from_id(get_current_channel_id())){
					std::cout << "sticking string in a Message and sending" << std::endl;
					//stick into Message and send
				}
				else{
					std::cout << "you need to be in a channel to do that" << std::endl;
				}
		}
	}
}

