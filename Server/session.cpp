#include "session.hpp"

Session::Session( tcp::socket socket, Channel::pointer channel ) : 
socket_( std::move( socket )), channel_( channel ) { 
	this->start();
}

Session::~Session( void ) { }

void Session::start( void ) { 
	channel_.join( shared_from_this());
}

void Session::deliver( const std::string & msg ) {
	bool writing = ! message_queue_.is_empty();
	message_queue_.push_back( msg );
	if ( ! writing ) {
		this->do_write_();
	}
}

void Session::do_read_( void ) {
	socket_.async_receive( boost::asio::buffer( read_buffer_ ), boost::bind( &Session::on_read_, shared_from_this(), _1, _2 ));
}

void Session::on_read_( const boost::system::error_code error, size_t bytes ) {
	if ( ! error ) {
		std::string message( std::move( read_buffer_ ), bytes );

		/* If there is no command, send the message to the client */
		channel_.deliver( message );
		{ /* . . .  */ }

		/* Else, process the command */

		{ /* . . . */  }


	}
}

/* Instead of string, should be message class */
void Session::do_write_( void ) {
	//std::strcpy( write_buffer_, message_queue_.front().c_str());
	socket_.async_send( boost::asio::buffer( this->message_queue_.front()), boost::bind( &Connection.on_write_, shared_from_this(), _1, _2 ));
}

void Session::on_write_( const boost::system::error_code error, size_t bytes ) {
	if ( ! error ) {
		do_read();
	}
}

