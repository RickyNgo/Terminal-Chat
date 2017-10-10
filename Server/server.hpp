#ifndef __SERVER_HPP__
#define __SERVER_HPP__

#include <boost/asio.hpp>
#include <boost/make_shared.hpp>
#include <boost/asio/signal_set.hpp>
#include <iostream>

/* For std::move */
#include <utility>
#include <list>

using boost::asio::ip::tcp;

class Server {

public:
	Server( boost::asio::io_service &, const tcp::endpoint & );
	~Server( void );

private:
	void accept( void );

	tcp::acceptor 		acceptor_;
	tcp::socket			socket_;
};

#endif