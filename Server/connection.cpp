#include "connection.hpp"

Connection::Connection( tcp::socket socket ) : socket_( std::move( socket )) { 
	this->start();
}

Connection::~Connection( void ) { }

void Connection::do_read( void ) {
	socket_.async_receive( boost::asio::buffer(read_buffer_), boost::bind( &Connection::on_read, shared_from_this(), _1, _2 ));
}

void Connection::on_read( const boost::system::error_code error, size_t bytes ) {
	if ( ! error ) {
		unsigned command = 0;
		
		/* Notes:

		Depending on how initial read is stored, the message could either be processed in an asynch post function
		or the message could be inspected for commands and, only when there is a command, would the message be processed
		with respect to the command.
	
		read - inspect - no command-> write to participants
				  |
				   - command -> process (possibly write to participants, sender, or not at all )

		*/

		std::string message( std::move( read_buffer_ ), bytes );
		
		/* Interpret Header Here...

		/* If there is no command, send the message to the client */
		if ( ! command ) {
			do_write( message );
		}

		/* Else, process the command */

		else { /* . . . */ }

	}
}

void Connection::do_write( const std::string & msg ) {
	socket_.async_send( boost::asio::buffer( msg ), boost::bind( &Connection::on_write, shared_from_this(), _1, _2 ));
}

void Connection::on_write( const boost::system::error_code error, size_t bytes ) {
	if ( ! error ) {
		do_read();
	}
}

void Connection::start( void ) { 
	this->do_read();
}

