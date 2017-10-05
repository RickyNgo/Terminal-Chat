#include "../hdr/server.hpp"

void Server::accept( void ) {
	while ( 1 ) {
		acceptor_.async_accept( socket_, [this] boost::system::error_code ec {
			if ( ec )
				std::cerr << "Error accepting socket. EC: " << ec << std::endl;
			else {

				/* Server - Client Integration Note:

				Read message for sender, add sender to pool
				
				socket_.async_receive();
				Channels.emplace_back( socket_, ** initiating particant ** );


				*/
			}
		});
	}
}

int main( int argc, char ** argv ) {
	if ( argc != 2 ) {
		std::cout << "Usage: " << argv[ 0 ] << " <port>" << std::endl;
		return 0;
	}
	try {
		boost::asio::io_service ios;
		boost::asio::endpoint endpoint( tcp::v4(), std::atoi( argv[ 1 ] ));
		Server server( ios, endpoint );
		ios.run();
	} catch( std::exception &e ) {
		std::cerr << e.what() << std::endl;
	}
	return 0;
}