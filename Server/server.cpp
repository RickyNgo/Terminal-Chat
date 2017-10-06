#include "server.hpp"

/* Server Constructor */
Server::Server( boost::asio::io_service &ios, const tcp::endpoint &endpoint ) {
	acceptor_ = tcp::acceptor( ios, endpoint );
	acceptor_.bind( endpoint, [ this ]boost::system::error_code ec {
		if ( ec ) {
			throw std::exception();
		}
	});
};


/* Server Destructor */
Server::Server( void ) { };

/* Acceptor - Transfers new connection to 'socket_' */
void Server::accept( void ) {
	while( 1 ) {
		try {
			acceptor_.async_accept( socket_, [ this ]boost::system::error_code ec {
				if ( ec ) {
					throw std::exception();
				} else {
					std::move( _socket ), 
				}
			});
		} catch ( std::exception &e ) {
			std::cerr << e.what() << std::endl;
		}
	}
}