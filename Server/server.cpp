#include "server.hpp"

/* Server Constructor */
Server::Server( boost::asio::io_service &ios, const tcp::endpoint &endpoint ) :
acceptor_( ios, endpoint ),
socket_( ios ),
ios_( ios )
{
	try {
		acceptor_.set_option( tcp::acceptor::reuse_address( true ));
		std::cerr << "Enki listening on " << boost::asio::ip::host_name() << ":" << endpoint.port() << ", press ctrl-c to terminate." << std::endl;
		acceptor_.listen();

		/* Start Async Accept Cycle */
		this->accept_();
		ios.run();
	} catch ( boost::system::system_error &e ) {
		std::cerr << "Acceptor: " << e.what() << std::endl;
	}
};


/* Server Destructor */
Server::~Server( void ) { };

// /***
//   * Server Async Accept Cycle
//   *
//   * accept_(): Calls the async_accept member function on socket and exits. async_accept() calls on_accept() when there is a connection.
//   * [ handler ]: (1) Calls process() to execute the command, (2) calls do_accept() to start cycle again.
//   **/

void Server::accept_( void ) {
	this->acceptor_.async_accept( this->socket_, [ this ]( boost::system::error_code error ) {

		/* If there is no error, process */ 
		if ( ! error ) {
			
		}
		this->accept_();
	});
}

void Server::recv_( void ) {
	this->socket_.async_receive( boost::asio::buffer( recv_buffer_, 1024 ),
		[ this ]( boost::system::error_code error, size_t bytes ) { 
			if ( ! error ) {
				ios_.post( boost::bind( &Server::process_, this ));
			}
			this->recv_();
	});
}

/* Any writing will usually be the result of an error */

void Server::send_( void ) {
	this->socket_.async_send( boost::asio::buffer( send_buffer_ ),
		[ this ]( boost::system::error_code error, size_t bytes ){
			if ( ! error ) {
				//
			}
		});
}

/* Process will take message as parameter */
void Server::process_( void ) {

	/* example */
	//add_service( this->ios_, new boost::asio::io_service );
}






