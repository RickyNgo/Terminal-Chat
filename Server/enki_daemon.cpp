/* Written by Brian
This the backend main function 
*/

#include "server.hpp"

int main( int argc, char ** argv ) {
	if ( argc != 2 ) {
		std::cout << "Usage: " << argv[ 0 ] << " <port>" << std::endl;
		return 0;
	}
	try {
		boost::asio::io_service ios;

		/* Initialize signal set -- ctrl-C terminates program */
		boost::asio::signal_set sig( ios, SIGINT );
		sig.async_wait( boost::bind( &boost::asio::io_service::stop, &ios ));

		/* Create Endpoint using servers IP and the port we want to listen on */
		tcp::endpoint endpoint( tcp::v4(), std::atoi( argv[ 1 ] ));
		Server server( ios, endpoint );
		ios.run();
	} catch( std::exception &e ) {
		std::cerr << e.what() << std::endl;
	}
	std::cerr << "\nTerminating Server." << std::endl;
	return 0;
}