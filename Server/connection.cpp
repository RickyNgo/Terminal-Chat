#include "connection.hpp"

/* Constructor */
Connection::Connection( tcp::socket socket, tcp::endpoint client, Processor & handler ) : 
/* The handler interacts with the connection via this guest object through virtual functions defined in this class */
Guest(),

/* The persistent connection with the user is through this socket */
socket_( std::move( socket )),

/* The clients ip (endpoint) */
client_( client ),

/* This class sends messages to the handler for processing since every message will have a command. */
handler_( handler )
{ }

Connection::~Connection( void ) {
	socket_.shutdown( tcp::socket::shutdown_both );
	socket_.close();
	std::cerr << "Connection " << get_id() << "::" << client_ << " terminated." << std::endl;
}

/* Initiate the connection */
void Connection::start( void ) {
	handler_.async_stage(
		shared_from_this(),
		boost::bind( &Connection::on_stage_, shared_from_this(), _1 )
	); 
}

void Connection::on_stage_( error_code ec ) {
	if ( ! ec ) {
		std::string temp = "Hello. Your connection # is " + get_id();
		( msg_.get_body()) = std::move( temp );
		std::cerr << msg_.get_body() << std::endl;
		do_write_();
	}
}

void Connection::on_login_( error_code ec ) {
	if ( ! ec ) {
		( msg_.get_body()) = "Login Successful!";
		do_write_();
	}
}

void Connection::do_read_header_( void ) {
	std::cerr << "In do read header" << std::endl;
	socket_.async_receive(
		boost::asio::buffer(
			read_buffer_,
			MAX_HEADER_LENGTH
		), boost::bind(
			&Connection::on_read_header_,
			shared_from_this(), 
			_1, _2 						/* errorcode, bytes read */
		)
	);
}

/* On Read, the message is passed to the handler for processing */
void Connection::on_read_header_( error_code error, size_t bytes ) {
	std::cerr << "On Read Header" << std::endl;
	( msg_.get_header()) = read_buffer_;
	std::cerr << "past get header" << std::endl;

	/* If there is no error, read the body */
	if ( ! error ) {
		do_read_body_();

	} else {
		if ( error.value() == boost::asio::error::eof ) {
			std::cerr << client_ << " >> EOF." << std::endl;
		}
	}
}

void Connection::do_read_body_( void ) {
	std::cerr << "Do read body" << std::endl;
	socket_.async_receive( 
		boost::asio::buffer( 
			read_buffer_,
			msg_.get_length()
		), boost::bind( 
			&Connection::on_read_body_,
			shared_from_this(),
			_1, _2
		)
	);
}

void Connection::on_read_body_( error_code error, size_t bytes ) {
	std::cerr << "on read body" << std::endl;
	(msg_.get_body()) = ( &read_buffer_[ 0 ] );
	// switch( msg_.get_command() ) {
	// 	case 1: /* TEMPORARY command for login */
	// 	handler_.async_login(
	// 		boost::asio::buffer( read_buffer_, msg_.get_length() ),
	// 		boost::bind( &Connection::on_login_, shared_from_this(), _1 )
	// 	);
	// 	std::cerr << client_ << " >> LOGIN." << std::endl;
	// 	break;
	// }
	handler_.async_login(
			boost::asio::buffer( read_buffer_, msg_.get_length() ),
			boost::bind( &Connection::on_login_, shared_from_this(), _1 )
		);
}

void Connection::do_write_( void ) {
	std::cerr << "in do_write_" << std::endl;
	std::cerr << "writing: " << msg_.get_body() << std::endl;
	socket_.async_send(
		boost::asio::buffer(
			msg_.get_body(),
			msg_.get_length()
		), boost::bind( 
			&Connection::on_write_,
			shared_from_this(),
			_1, _2
		)
	);
}


void Connection::on_write_( boost::system::error_code error, size_t bytes ) {
	if ( ! error ) {
		std::cerr << bytes << " bytes >> " << client_ << "." << std::endl;
	} else {
		std::cerr << "Connection Error: on_write_: " << error << std::endl;
	}
	
	do_read_header_();
}