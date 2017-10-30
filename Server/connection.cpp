#include "connection.hpp"

/* Constructor */
Connection::Connection( tcp::socket socket, tcp::endpoint client, ConnectionHandler & handler ) : 
/* The handler interacts with the connection via this guest object through virtual functions defined in this class */
Guest(),

/* The persistent connection with the user is through this socket */
socket_( std::move( socket )),

/* The clients ip (endpoint) */
client_( client ),

/* This class sends messages to the handler for processing since every message will have a command. */
handler_( handler )
{ }

/* Initiate the connection */
void Connection::start( void ) {

	/* Handler creates shared pointer to this connection - 2 pointers now: 1. Server, 2. Handler */
	handler_.join( shared_from_this());
	std::cerr << "Ready to receive data from " << client_ << std::endl;

	/* Start async recv-send cycle */
	do_read_();
}

void Connection::do_read_( void ) {
	socket_.async_receive( boost::asio::buffer( recv_buffer_, sizeof( send_buffer_ )), boost::bind( &Connection::on_read_, shared_from_this(), _1, _2 ));
}

void Connection::do_write_( void ) {
	std::cerr << "Echoing " << send_buffer_ << std::endl;
	socket_.async_send( boost::asio::buffer( send_buffer_, strlen( send_buffer_ )), boost::bind( &Connection::on_write_, shared_from_this(), _1, _2 ));
}

/* On Read, the message is passed to the handler for processing */
void Connection::on_read_( boost::system::error_code error, size_t bytes ) {

	/* If there is no error, send the message */
	if ( ! error ) {
		std::cerr <<  client_ << " >> " << bytes << " bytes." << std::endl;

		/* ECHO SERVER */
		std::strcpy( send_buffer_, recv_buffer_);
		send_buffer_[ strlen( recv_buffer_) ] = '\0';
		memset( recv_buffer_, '\0', sizeof( recv_buffer_ ));
		do_write_();
		// handler_.request( std::move( recv_buffer_ ), shared_from_this());
	} else {
		if ( error.value() == boost::asio::error::eof ) {
			std::cerr << client_ << " >> EOF." << std::endl;
		}
	}
}

void Connection::on_write_( boost::system::error_code error, size_t bytes ) {
	if ( ! error ) {
		std::cerr << bytes << " bytes >> " << client_ << "." << std::endl;
	} else {
		std::cerr << "Connection Error: on_write_: " << error << std::endl;
	}
	std::memset( send_buffer_, '\0', sizeof( send_buffer_ ));
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