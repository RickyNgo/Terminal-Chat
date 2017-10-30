#include "server.hpp"

/* Server Constructor */


Server::Server( const char * host, const short & port ) :
/* Create the io_service */
ios_(),

/* Create the endpoint for the server ( localhost:port ) */
address_( boost::asio::ip::address::from_string( host ), port ),

/* Create the acceptor. The acceptor listens for incoming connections */
acceptor_( ios_, address_ ),

/* Create the socket that will accept the data connection created by the acceptor */
socket_( ios_ ),

/* Create signal set that will be used to determine when to stop the server */
signals_( ios_, SIGINT )
{
	try {
		acceptor_.set_option( tcp::acceptor::reuse_address( true ));
		std::cerr << "Enki listening on " << boost::asio::ip::host_name();
		std::cerr << " [" << this->address_ << "], press ctrl-c to terminate." << std::endl;
		acceptor_.listen();
		do_stop_();

		/* Start Async Accept Cycle */
		accept_();

	} catch ( boost::system::system_error &e ) {
		std::cerr << "Constructor: " << e.what() << std::endl;
	}
};


/* Server Destructor */
Server::~Server( void ) { };

/* Start Server */
void Server::start( void ) {
	ios_.run();
}

/* Creates handler that will only be executed once SIGINT is received */
void Server::do_stop_( void ) {
	signals_.async_wait( boost::bind( &Server::on_stop_, this ));
}

/* Handler function for do_stop_. This function closes all connections in the hanlder */
void Server::on_stop_( void ) {
	std::cerr << "\n\nInitiating Server Shutdown." << std::endl;
	std::cerr << "\nClosing all connections..." << std::endl;
	handler_.stop_all();
	std::cerr << "** Complete **" << std::endl;
	ios_.stop();
	std::cerr << "Server shutdown sucessful." << std::endl;
}

/***
  * Server Async Accept Cycle
  * Call accept and pass lambda as handler. Once complete, lambda calls accept again to begin the cycle again.
  */

void Server::accept_( void ) {
	this->acceptor_.async_accept( this->socket_, this->client_,
		[ this ]( boost::system::error_code error ) {

		/* If there is no error, call create connection to start receiving data */
		if ( ! error ) {
			std::cerr << "Hello " << this->client_ << "." << std::endl;

			/* It is recommended (Torjo, John. Boost.Asio C++ Network Programming. Packt Publishing, 2013. pp.31-33, 53-54 )
			to create a connection class to handle asynchronous reads and writes with a socket. Alternatively, stack space
			can be passed with the completion handler to boost::bind to ensure valid memory use. */

			/* The server points to a newly created shared connection and calls start */
			boost::make_shared<Connection>( std::move( socket_ ), std::move( client_ ), handler_ )->start();

			/* At this point, the shared pointer above will go out of scope. The handler will be the main owner of
			the new connection */

		} else { 
			std::cerr << "Connection Error: " << error << "." << std::endl;
		}

		/* Call accept to be async accept cycle again */
		this->accept_();
	});
}





