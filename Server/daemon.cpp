/*
This the backend main function 
*/

#include "server.hpp"
#include <list>

int main( int argc, char ** argv ) {
	if ( argc != 2 ) {
		std::cout << "Usage: " << argv[ 0 ] << " <port>" << std::endl;
		return 0;
	}
	try {
		/* Create the IO Service Master Object */
		boost::asio::io_service ios_master;

		/* Initialize signal set -- ctrl-c terminates program */
		boost::asio::signal_set sig( ios_master, SIGINT );
		sig.async_wait( boost::bind( &boost::asio::io_service::stop, &ios_master ));

		/* Create Endpoint with localhost (using "127.0.0.1" as described in 
		Torjo, John. Boost.Asio C++ Network Programming. Packt Publishing, 2013. p.22) */
		tcp::endpoint endpoint( boost::asio::ip::address::from_string( "127.0.0.1" ), std::atoi( argv[ 1 ] ));

		/* Pass in endpoint and ios_master to initialize the server. Server calls ios.run() */
		Server enki( ios_master, endpoint );

	} catch( std::exception & error ) {
		std::cerr << error.what() << std::endl;
	}
	std::cerr << "\nTerminating Server." << std::endl;
	return 0;
}