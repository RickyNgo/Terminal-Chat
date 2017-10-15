#ifndef __SERVER_HPP__
#define __SERVER_HPP__

#include <boost/asio.hpp>
using boost::asio::ip::tcp;

#include <boost/make_shared.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>

#include <iostream>
#include <string>
#include <utility>

class Server {
public:
	Server( boost::asio::io_service &, const tcp::endpoint & );
	~Server( void );

	void start( void );

private:
	void accept_( void );
	void recv_( void );
	void send_( void );
	void process_( void );

	char recv_buffer_[ 1024 ];
	char send_buffer_[ 1024 ];

	tcp::acceptor 				acceptor_;
	tcp::socket					socket_;
	boost::asio::io_service & 	ios_;
};

#endif