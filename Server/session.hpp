#ifndef __SESSION_HPP__
#define __SESSION_HPP__

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <string>

#include "channel.hpp"
#include "../Messages/messages.hpp"
#include "guest.hpp"
#include <queue>


using boost::asio::ip::tcp;

class Session :
public boost::enable_shared_from_this<Session> {
public:
	Session( Guest::pointer guest, tcp::socket socket, const short, Channel::pointer channel );
	~Session( void );
	typedef boost::shared_ptr<Session> pointer;
	typedef boost::system::error_code error_code;

    void deliver( Messages & msg );
    void start( void );

private:
	void do_connect_( void );
	void on_connect_( error_code );

	void do_read_header_();
	void on_read_header_(const boost::system::error_code error, size_t bytes);

	void do_read_body_();
	void on_read_body_(const boost::system::error_code error, size_t bytes);

	void do_write_header_();
	void on_write_header_(const boost::system::error_code error, size_t bytes);
	
	void do_write_body_();
	void on_write_body_(const boost::system::error_code error, size_t bytes);

	char read_buffer_[ 512 ];
	Messages read_msg;
	std::queue <Messages> write_msg;
	boost::recursive_mutex write_msg_m_;

	tcp::socket socket_;
	Channel::pointer room_;
	const short 	port_;
	Guest::pointer	 guest_;
	tcp::endpoint 	address_;
};

#endif