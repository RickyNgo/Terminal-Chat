#ifndef __CONNECTION_HPP__
#define __CONNECTION_HPP__

#include <boost/asio.hpp>
using boost::asio::ip::tcp;
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <iostream>
#include <vector>
#include <ctime>

#include "processor.hpp"
#include "messages.hpp"
#include "guest.hpp"

#define HEADER_LEN 2

class Connection : 
public Guest,
public boost::enable_shared_from_this<Connection>,
public boost::noncopyable {
public:
	typedef boost::shared_ptr<Connection> pointer;
	typedef boost::system::error_code error_code;
	
	Connection( tcp::socket, tcp::endpoint, Processor & );
	~Connection( void );
	void start( void );

private:
	void on_login_( error_code );
	void on_stage_( error_code );

	void do_read_header_( void );
	void on_read_header_( error_code, size_t );

	void do_read_body_( Messages );
	void on_read_body_( error_code, size_t );

	void do_write_( Messages );
	void on_write_( error_code, size_t );
	
	Messages 				msg_;
	char					read_buffer_[ 512 ];
	tcp::socket				socket_;
	tcp::endpoint			client_;
	Processor		 		& handler_;
};

#endif  /* END CONNECTION */