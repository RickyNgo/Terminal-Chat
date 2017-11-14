#ifndef __SESSION_HPP__
#define __SESSION_HPP__

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "messages.hpp"
#include <queue>


using boost::asio::ip::tcp;

class Channel;

class Session :
public boost::enable_shared_from_this<Session> {
public:
	Session( tcp::socket socket, Channel &room);
	~Session( void );
	typedef boost::shared_ptr<Session> pointer;
    
    //void deliver( const std::string & );
	void start();

	void do_read_header();
	void on_read_header(const boost::system::error_code error, size_t bytes);

	void do_read_body();
	void on_read_body(const boost::system::error_code error, size_t bytes);

	void do_write_header();
	void on_write_header(const boost::system::error_code error, size_t bytes);
	
	void do_write_body();
	void on_write_body(const boost::system::error_code error, size_t bytes);
	
	

private: 
	std::vector<char> read_buffer;
	Messages read_msg;
	std::queue <Messages> write_msg;
	tcp::socket socket_;
	Channel &room_;
};

#endif