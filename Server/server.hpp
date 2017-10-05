#ifndef __SERVER_HPP__
#define __SERVER_HPP__

#include <sys/poll.h>
#include <boost/asio.hpp>
#include <list>

using boost::asio::ip::tcp;

class Server {
private:
	tcp::socket 		socket_;
	tcp::acceptor 		acceptor_;
	std::list<Channel> 	channels_;

	void accept( void );
	void signon( void );  /* Handle User Signons */
	void close( void );  /* Close channel */

public:
	Server( boost::asio::io_service &ios, const tcp::endpoint &endp ) :
	acceptor_( ios, endp ),
	socket_( endp ) { };

	~Server( void ) { };

};

#endif