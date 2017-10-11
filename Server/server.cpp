#include "server.hpp"

/* Server Constructor */
Server::Server( boost::asio::io_service &ios, const tcp::endpoint &endpoint ) :
acceptor_( ios, endpoint ),
socket_( ios ) {
	try {
		acceptor_.set_option( tcp::acceptor::reuse_address( true ));
		std::cerr << "Enki Server listening on port " << endpoint.port() << "." << std::endl;
		acceptor_.listen();

		/* Start Async Accept Cycle */
		this->accept();
	} catch ( boost::system::system_error &e ) {
		std::cerr << "Acceptor: " << e.what() << std::endl;
	}
};


/* Server Destructor */
Server::~Server( void ) { };


/***
  * Async Accept Cycle
  *
  * Accept() begins the async call-back cycle. When async_accept() completes, it is the handlers job (async_accept_h()) to 
  * call accept again upon success of the prior operation.
  */

void Server::accept( void ) {
	try {
		this->acceptor_.async_accept( this->socket_, [ this ]( boost::system::error_code error ) {
			if ( ! error ) {
				this->accept();
			}
		});
	} catch ( boost::system::system_error &e ) {
		std::cerr << "Run: " << e.what() << std::endl;
	}
}