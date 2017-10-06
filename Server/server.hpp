#ifndef __SERVER_HPP__
#define __SERVER_HPP__

#include <boost/asio.hpp>
/* For std::move */
#include <utility>
#include <list>

using boost::asio::ip::tcp;

class Server {
private:
	tcp::socket 		socket_;
	tcp::acceptor 		acceptor_;

public:
	Server( boost::asio::io_service, const tcp::endpoint );
	~Server( void ) { };
};

#endif