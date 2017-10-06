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
		tcp::endpoint endpoint( tcp::v4(), std::atoi( argv[ 1 ] ));
		Server server( ios, endpoint );
		ios.run();
	} catch( std::exception &e ) {
		std::cerr << e.what() << std::endl;
	}
	return 0;
}