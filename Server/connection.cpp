#include "connection.hpp"

/* Constructor */
Connection::Connection( tcp::socket socket, tcp::endpoint client, ConnectionHandler & handler ) : 
Guest(),
socket_( std::move( socket )),
client_( client ),
handler_( handler )
{ }

void Connection::start( void ) {

	/* Start reading from connection */
	handler_.join( shared_from_this());
	do_read_();
}

void Connection::do_read_( void ) {
	std::cerr << "Connection wait for message from " << client_ << std::endl;
	socket_.async_receive( boost::asio::buffer( recv_buffer_, 1024 ), boost::bind( &Connection::on_read_, shared_from_this(), _1, _2 ));
}

void Connection::do_write_( void ) {
	socket_.async_send( boost::asio::buffer( send_buffer_, 1024 ), boost::bind( &Connection::on_write_, shared_from_this(), _1, _2 ));
}

void Connection::on_read_( boost::system::error_code error, size_t bytes ) {
	if ( ! error ) {
		std::cerr << "Read " << bytes << " bytes from " << client_ << std::endl;
		handler_.request( boost::make_shared<Messages>( std::move( recv_buffer_ )), shared_from_this());
	} else {
		do_read_();
	}
}

void Connection::on_write_( boost::system::error_code error, size_t bytes ) {
	if ( ! error ) {
		// write success
	}
	do_read_();
}

void Connection::quit( void ) {
	socket_.close();
	std::cerr << "Bye " << this->get_alias() << "[" << client_ << "]." << std::endl;
}