#include <boost/asio.hpp>
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
}
/*
write
*/
//void Client::write(){
//    ios.post(
//        [this, msg]()
//        {
//          bool write_in_progress = !write_msgs_.empty();
//          write_msgs_.push_back(msg);
//          if (!write_in_progress)
//          {
//            do_write_();
//          }
//        });
//}

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

/*
get_channels
*/
std::vector<Channel*> Client::get_channels(){return client_channels_;}

tcp::socket* Client::get_main_socket(){return &main_socket_;}


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
set_room_list
*/
void Client::set_channel_list(std::vector<Channel *> chan_list){client_channels_ = chan_list;}

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
void Client::add_channel(Channel* chan){
    client_channels_.push_back(chan);
}


/*
remove_room
this is just removing the room from the vector, not actually closing the room. either do that in a separate function or do it here
*/
void Client::remove_channel(Channel* chan){
	client_channels_.erase(std::remove(client_channels_.begin(), client_channels_.end(), chan), client_channels_.end());
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

    std::cout << "in else statement" << std::endl;
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
//void Client::do_read_body_(){
//    boost::asio::async_read(main_socket_,
//    boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
//    [this](boost::system::error_code ec, std::size_t /*length*/)
//    {
//      if (!ec){
//        std::cout.write(read_msg_.body(), read_msg_.body_length());
//        std::cout << "\n";
//        do_read_header_();
//      }
//      else{
//        main_socket_.close();
//      }
//    });
//}

/*
do_write_
*/
//void do_write_(){
//    boost::asio::async_write(main_socket_,
//    boost::asio::buffer(write_msgs_.front().data(),
//      write_msgs_.front().length()),
//    [this](boost::system::error_code ec, std::size_t /*length*/)
//    {
//      if (!ec)
//      {
//        write_msgs_.pop_front();
//        if (!write_msgs_.empty())
//        {
//          do_write_();
//        }
//      }
//      else
//      {
//        main_socket_.close();
//      }
//    });
//}


