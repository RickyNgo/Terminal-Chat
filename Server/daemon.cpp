/*
This the backend main function 
*/

#include "server.hpp"

int main( int argc, char ** argv ) {
	if ( argc != 3 ) {
		std::cout << "Usage: " << argv[ 0 ] << " <command-port> <session-port>" << std::endl;
		return 0;
	}
	try {

		/* Create server using localhost (using "127.0.0.1" as described in 
		Torjo, John. Boost.Asio C++ Network Programming. Packt Publishing, 2013. p.22),
		and port number supplied on command-line */

		Server enki( "127.0.0.1", std::atoi( argv[ 1 ] ), std::atoi( argv[ 2 ] ));
		enki.start();

	} catch( std::exception & error ) {
		std::cerr << error.what() << std::endl;
	}

	std::cerr << "Program terminated sucessfully." << std::endl;
	return 0;
}