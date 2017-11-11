#ifndef __SERVER_HPP__
#define __SERVER_HPP__

#include <boost/asio.hpp>
using boost::asio::ip::tcp;

#include <boost/make_shared.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/bind.hpp>
#include <cstring>
#include <iostream>
#include <string>
#include <utility>

#include "connection.hpp"
#include "processor.hpp"

class Server {
public:
	Server( const char * host, const short & port );
	~Server( void );

	void start( void );

private:
	void accept_( void );
	void do_stop_( void );
	void on_stop_( void );

	boost::asio::io_service 	ios_;
	tcp::endpoint				address_;
	tcp::acceptor 				acceptor_;
	tcp::socket					socket_;
	tcp::endpoint				client_;
	boost::asio::signal_set 	signals_;
	Processor					handler_;

};

#endif