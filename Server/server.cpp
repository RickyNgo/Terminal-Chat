#include "server.hpp"

/* Server Constructor */
Server::Server( const char * host, const short & port ) :
ios_(),
address_( boost::asio::ip::address::from_string( host ), port ),
acceptor_( ios_, address_ ),
socket_( ios_ ),
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

void Server::do_stop_( void ) {
	signals_.async_wait( boost::bind( &Server::on_stop_, this ));
}

void Server::on_stop_( void ) {
	std::cerr << "\nClosing all connections...\n";
	handler_.stop_all();
	std::cerr << "complete." << std::endl;
	ios_.stop();
	std::cerr << "Server shutdown sucessful." << std::endl;
}

/***
  * Server Async Accept Cycle
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

			boost::make_shared<Connection>( std::move( socket_ ), std::move( client_ ), handler_ )->start(); 

		} else { 
			std::cerr << "Connection Error: " << error << "." << std::endl;
		}
		this->accept_();
	});
}





