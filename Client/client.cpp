#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <cstdlib>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <thread>


#include "client.hpp"


using boost::asio::ip::tcp;


Client::Client(boost::asio::io_service& io_serv, tcp::resolver::iterator endpoint_iterator)
:ios(io_serv),main_socket_(io_serv), user_alias_(""), user_id_(-1), current_channel_(0){
    do_connect_(endpoint_iterator);
    
    //choose_alias();
}

void Client::choose_alias(){
    std::string alias, alias_req, u_input, command;
    char ret_msg[512];
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
            Messages alias_construct("", alias, current_time, LOGIN);
            alias_req = alias_construct.encode();
            
            valid_name = true;
            
            
            
            // send to server
            do_write_(alias);
            
            // get response
            do_read_body_();

            
            
            std::cout << "am i here?" << std::endl;
            std::cout << "receive message length: " << strlen(ret_msg) << std::endl;
            
            
//            for(int i = 0; i < strlen(ret_msg); i++){
//                std::cout << ret_msg[i] << std::endl;
//            }
//            
//            
//            // decode with Message
//            Messages alias_decode(ret_msg);
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

/*
write
*/
void Client::write(std::string msg){
    // send to server
//    std::cout << "alias req length: " << msg.length() << std::endl;
//    boost::asio::async_write(*(c.get_main_socket()), boost::asio::buffer(alias_req, alias_req.length()), handler);
//    io_service.run();
    //c.get_main_socket()->async_send(boost::asio::buffer(alias_req, alias_req.length()), handler);

}

void Client::handler(const boost::system::error_code& error, std::size_t bytes_transferred){
    if (!error){
        std::cout << "write success\n";
        std::cout << "bytes written: " << bytes_transferred << std::endl;
    }
    else{
        std::cout << "write not a success\n";
        std::cout << "category: " << error.category().name() << " code: " << error.value() << " message: " << error.message() << std::endl;
        std::cout << "bytes written: " << bytes_transferred << std::endl;
    }
}

/*
close
*/
void Client::close(){
	ios.post([this](){ 
		main_socket_.close();
	});
}

/*
show_help()
*/
std::string Client::show_help(){
	std::string help = "Here is some basic help output";  //how to print this in correct place using ncurses?

	return help;
}

/*
get_user_alias
*/
std::string Client::get_user_alias(){return user_alias_;}

/*
get_user_id
*/
int Client::get_user_id(){return user_id_;}

/*
get_friend_list
*/
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


/*
set_user_alias
*/
void Client::set_user_alias(std::string name){user_alias_ = name;} //perform validation outside of class

/*
set_user_id
*/
void Client::set_user_id(int id){user_id_ = id;}

/*
set_friend_list
*/
//change this to whatever the server sends the friend list as
void Client::set_friend_list(std::map<int, std::string> friends){friend_list_ = friends;} 

/*
set current channel
*/
void Client::set_current_channel(Channel* chan){current_channel_ = chan->get_channel_id();}

/*
add_friend
*/
void Client::add_friend(int id, std::string name){ //http://www.cplusplus.com/reference/map/map/insert/
    std::pair<std::map<int,std::string>::iterator,bool> ret;
    ret = friend_list_.insert(std::pair<int,std::string>(id,name));
	
	if (ret.second==false) {
		std::cout << "friend already added." << '\n'; // update this
	}
}

/*
add_room
*/
// replace parameter with ChatRoom
void Client::add_channel(Channel* chan, int id){
	std::pair<std::map<int,Channel*>::iterator,bool> ret;
	ret = client_channels_.insert (std::pair<int,Channel*>(id,chan));
	if (ret.second==false) {
		std::cout << "That channel already exists." << std::endl;
	}
}


/*
remove_room
this is just removing the room from the vector, not actually closing the room. either do that in a separate function or do it here
*/
void Client::remove_channel(int id){
	client_channels_.erase(id);
}

/*
setup_client
this is going to need to partly moved to the constructor and partly broken out into other functions for db stuff. 
 either leave this function to corral them all, or do it somewhere else.
*/
void Client::setup_client(std::string name, int id){
	set_user_alias(name);
	set_user_id(id);

	//ask server for friends and chatrooms
    //update: connect to db and get info
}

//custom connect for testing using handler
/*
 do_connect_
 */
void Client::do_connect_(tcp::resolver::iterator endpoint_iterator){
	boost::asio::async_connect(main_socket_, endpoint_iterator,
        [this](boost::system::error_code ec, tcp::resolver::iterator){
            if (!ec){
            	show_help();	
        	}
    });
}

// next four come straight from example

/*
do_connect_
*/
//void Client::do_connect_(tcp::resolver::iterator endpoint_iterator){
//	boost::asio::async_connect(main_socket_, endpoint_iterator,
//        [this](boost::system::error_code ec, tcp::resolver::iterator)
//        {
//          if (!ec)
//          {
//            do_read_header_();
//          }
//        });
//}

/*
do_read_header_
*/
//void Client::do_read_header_(){
//	boost::asio::async_read(main_socket_,
//        boost::asio::buffer(read_msg_.data(), chat_message::header_length),
//        [this](boost::system::error_code ec, std::size_t /*length*/)
//        {
//          if (!ec && read_msg_.decode_header()){
//            do_read_body_();
//          }
//          else{
//            main_socket_.close();
//          }
//        });
//}

/*
do_read_body_
*/
void Client::do_read_body_(){
    char ret_msg[512];
    boost::asio::async_read(main_socket_,
    boost::asio::buffer(ret_msg, 512),
        [this](boost::system::error_code ec, std::size_t bytes_transferred){
            if (!ec){
                std::cout << "write success\n";
                std::cout << "bytes written: " << bytes_transferred << std::endl;
                
                
                }
            else{
                std::cout << "write not a success\n";
                std::cout << "category: " << ec.category().name() << " code: " << ec.value() << " message: " << ec.message() << std::endl;
                std::cout << "bytes written: " << bytes_transferred << std::endl;
                }
            });
}

/*
do_write_
*/
void Client::do_write_(std::string msg){
    // send to server
    std::cout << "alias req length: " << msg.length() << std::endl;
    
//    boost::asio::async_write(main_socket_, boost::asio::buffer(msg, msg.length()), boost::bind(&Client::handler, shared_from_this(), _1, _2));
//    
//        boost::asio::write(main_socket_, boost::asio::buffer(msg, msg.length()));

    boost::asio::async_write(main_socket_, boost::asio::buffer(msg, msg.length()),
                             [this](boost::system::error_code ec, std::size_t bytes_transferred){
                             if (!ec){
                                 std::cout << "write success\n";
                                 std::cout << "bytes written: " << bytes_transferred << std::endl;
                                 do_read_body_();
                                 
                             }
                             else{
                                 std::cout << "write not a success\n";
                                 std::cout << "category: " << ec.category().name() << " code: " << ec.value() << " message: " << ec.message() << std::endl;
                                 std::cout << "bytes written: " << bytes_transferred << std::endl;
                             }
                             });
    
        std::cout << "bytes available " << main_socket_.available() << std::endl;
    
//    ios.run();
}


