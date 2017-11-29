
#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include "../Ncurses/layout.hpp"


using boost::asio::ip::tcp;

#include "channel.hpp"

Channel::Channel(std::string name, int id, boost::asio::io_service& ios, int port):
    channel_name(name), 
    channel_id(id), 
    role(NO_ROLE), 
    type(NO_TYPE),
    port(port),
    channel_socket_(ios),
    acceptor(ios, tcp::endpoint(tcp::v4(), port)) 
    {
    }

Channel::~Channel(){}

void Channel::start()
{
	acceptor.listen();
	
	acceptor.async_accept(channel_socket_, boost::bind(&Channel::accept_handler, shared_from_this(), _1));
}

void Channel::accept_handler(const boost::system::error_code& error)
{
	if (!error)
	{
        std::ofstream log;
        log.open("log.txt");

		log << "The channel has accepted a session" << channel_socket_.is_open() << std::endl;
		time_t current_time;
		Messages test("Client", "I have joined", time(&current_time), MSG);
		
		write_queue.push(test); 
		do_write_header();

        log.close();
	}
	else
	{
        std::ofstream log;
        log.open("log.txt");
		log << error.message() << std::endl;
        log.close();
	}
}

/***************************************
 on_read_header
 
 read handler
 ***************************************/
void Channel::on_read_header( boost::system::error_code ec, std::size_t bytes ) {
    if (!ec) {
        read_msg.get_header() = read_buffer_;
        read_msg.parse_header();
        /*
        Messages msg;
        msg.get_header() = read_buffer_;
        
        msg.parse_header();
        body_length_ = msg.get_length();
        command_ = msg.get_command();
        */

        body_length_ = read_msg.get_length();
        command_ = read_msg.get_command();
        
        //std::cout << "header body length: " << read_msg.get_length() << "by member: " << body_length_ << std::endl;
        //std::cout << "header command: " << read_msg.get_command() << "by member: " << command_ << std::endl;
               
        do_read_body();
    } else {
        //std::cout << "Read error in on_read_header: " << ec << std::endl;
    }
}

/***************************************
 on_read_body
 
 read handler
 ***************************************/
void Channel::on_read_body( boost::system::error_code ec, std::size_t bytes ) {
    if (!ec) {
        //std::cout << "read_buffer_: " << this->read_buffer_ << std::endl;
        read_msg.get_body() = read_buffer_;
        //std::cout << "Read body: " << read_msg.get_body() << std::endl;
        //memset(read_buffer_, '\0', sizeof(char)*512);
        
        // UNCOMMENT THIS!!
        time_t temp = read_msg.get_time();

        struct tm *time_info = localtime(&temp);
        char *raw_time = new char[12];

        sprintf(raw_time, "%02d:%02d:%02d|", time_info->tm_hour, time_info->tm_min, time_info->tm_sec);

        std::string fmtd_time(raw_time);
        update_buffers(fmtd_time, read_msg.get_sender(), read_msg.get_body());
        do_read_header();
        display_chat();
    } else {
        //std::cout << "Read error: " << ec << std::endl;
    }
}

/***************************************
 do_read_header
 
 ***************************************/
void Channel::do_read_header() {
    read_msg.clear();
    memset(read_buffer_, '\0', sizeof(char)*512);
    channel_socket_.async_receive(
                               boost::asio::buffer(
                                                   read_buffer_,
                                                   MAX_HEADER_LENGTH),
                               boost::bind(&Channel::on_read_header,
                                           shared_from_this(),
                                           _1, _2 ));
    
    //std::cerr << "called handler on_read_header" << std::endl;
    
}

/***************************************
 do_read_body
 
 ***************************************/
void Channel::do_read_body() {
    memset(read_buffer_, '\0', sizeof(char)*512);
    channel_socket_.async_receive(
                               boost::asio::buffer(
                                                   read_buffer_,
                                                   read_msg.get_length()),
                               boost::bind(&Channel::on_read_body,
                                           shared_from_this(),
                                           _1, _2 ));
    
    //std::cerr << "called handler on_read_body" << std::endl;
    
}

/*****************************************************/

void Channel::on_write_header( boost::system::error_code ec, std::size_t bytes ) {
    if (!ec) {       
        std::ofstream log;
        log.open("log.txt");
		log << "ON WRITE HEADER " << ec.message() << std::endl;
        log.close();
        do_write_body();
    } else {
        //std::cout << "Read error in on_read_header: " << ec << std::endl;
        std::ofstream log;
        log.open("log.txt");
		log << ec.message() << std::endl;
        log.close();
    }
}

void Channel::on_write_body( boost::system::error_code ec, std::size_t bytes ) {
    if (!ec) {

        write_queue.pop();
        if (write_queue.empty())
        {
            std::ofstream log;
            log.open("log.txt");
		    log << "ON WRITE BODY " << ec.message() << std::endl;
            log.close();
            do_read_header();
        }
        else
        {
            do_write_header();
        }
        
    } else {
        //std::cout << "Read error: " << ec << std::endl;
    }
}

void Channel::do_write_header() {

    boost::asio::async_write(channel_socket_,
                               boost::asio::buffer(
                                                   write_queue.front().get_header(),
                                                   MAX_HEADER_LENGTH),
                               boost::bind(&Channel::on_write_header,
                                           shared_from_this(),
                                           _1, _2 ));
    
    //std::cerr << "called handler on_read_header" << std::endl;
}

void Channel::do_write_body() {

    std::ofstream log;
    log.open("log.txt");
	log << "DO WRITE BODY " << std::endl;
    log.close();

    boost::asio::async_write(channel_socket_,
                               boost::asio::buffer(
                                                   write_queue.front().get_body(),
                                                   write_queue.front().get_length()),
                               boost::bind(&Channel::on_write_body,
                                           shared_from_this(),
                                           _1, _2 ));
    
    //std::cerr << "called handler on_read_body" << std::endl;
}

void Channel::set_channel_name(std::string name){
    channel_name = name;
}

void Channel::set_channel_id(int id){
    channel_id = id;
}

void Channel::set_channel_socket(tcp::socket* socket){
    //channel_socket_ = socket;
}

void Channel::set_channel_type(ChannelType ct){
    type = ct;
}

void Channel::set_channel_role(ChannelRole cr){
    role = cr;
}

std::string Channel::get_channel_name(){
    return channel_name;
}

int Channel::get_channel_id(){
    return channel_id;
}

tcp::socket& Channel::get_channel_socket(){
    return channel_socket_;
}

ChannelType Channel::get_channel_type(){
    return type;
}

ChannelRole Channel::get_channel_role(){
    return role;
}
