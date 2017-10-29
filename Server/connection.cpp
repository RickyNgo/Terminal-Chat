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
	std::cerr << "Ready to receive data from " << client_ << std::endl;
	do_read_();
}

void Connection::do_read_( void ) {
	socket_.async_receive( boost::asio::buffer( recv_buffer_, strlen( send_buffer_ )), boost::bind( &Connection::on_read_, shared_from_this(), _1, _2 ));
}

void Connection::do_write_( void ) {
	socket_.async_send( boost::asio::buffer( send_buffer_, strlen( send_buffer_ )), boost::bind( &Connection::on_write_, shared_from_this(), _1, _2 ));
}

void Connection::on_read_( boost::system::error_code error, size_t bytes ) {
	if ( ! error ) {
		std::cerr <<  client_ << " >> " << bytes << " bytes." << std::endl;

		handler_.request( std::move( recv_buffer_ ), shared_from_this());
	} else {
		std::cerr << "Connection Error: on_read_: " << error << std::endl;
	}
}

void Connection::on_write_( boost::system::error_code error, size_t bytes ) {
	if ( ! error ) {
		std::cerr << bytes << " bytes >> " << client_ << "." << std::endl;
	} else {
		std::cerr << "Connection Error: on_write_: " << error << std::endl;
	}
	do_read_();
}

void Connection::response( boost::shared_ptr<Messages> msg ) {
	std::string m = msg->encode();
	std::strcpy( send_buffer_, m.c_str());
	do_write_();
}

void Connection::quit( void ) {
	socket_.shutdown( tcp::socket::shutdown_both );
	socket_.close();
	std::cerr << "Connection " << get_id() << "::" << client_ << "." << std::endl;
}