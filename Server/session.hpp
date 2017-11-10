#ifndef __SESSION_HPP__
#define __SESSION_HPP__

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "connection.hpp"
#include "messages.hpp"
#include <queue>

using boost::asio::ip::tcp;

class Session :
public boost::enable_shared_from_this<Session> {
public:
	Session( tcp::socket socket, Channel::pointer channel );
	~Session( void );
	typedef boost::shared_ptr<Session> pointer;
    
    //void deliver( const std::string & );
	void start();
	void relay_msg();

	void do_read();
	void on_read_header(const boost::system::error_code error, size_t bytes);

	void do_read_body();
	void on_read_body(const boost::system::error_code error, size_t bytes);

	void do_write();
	void on_write(const boost::system::error_code error, size_t bytes);
	

private: 
	Messages read_msg;
	std::queue <Messages> write_msg;
	tcp::socket socket;
	Channel room;
};

#endif