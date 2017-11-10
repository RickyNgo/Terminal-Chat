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
		std::sprintf( send_buffer_ + 6, "Hello. Your connection # is %d.\0", get_id());
		std::printf( "%s\n", send_buffer_ + 6 );
		send_buffer_[ 0 ] = static_cast<unsigned char>( strlen( send_buffer_ + 2 ));
		send_buffer_[ 1 ] = static_cast<unsigned char>( 2 );
		sprintf( send_buffer_ + 2, "%d", get_id());
		std::printf( "%s\n", send_buffer_ + 2 );
		do_write_();
	}
}

void Connection::on_login_( error_code ec ) {
	if ( ! ec ) {
		std::sprintf( send_buffer_ + 6, "Hi %s, your login was successful!", get_alias());
		std::printf( "%s\n", send_buffer_ + 6 );
		send_buffer_[ 0 ] = static_cast<unsigned char>( strlen( send_buffer_ + 2 ));
		send_buffer_[ 1 ] = static_cast<unsigned char>( 1 );
		sprintf( send_buffer_ + 2, "%u", get_id() );
		std::printf( "%s\n", send_buffer_ + 6 );

		do_write_();
	}
}

void Connection::do_read_header_( void ) {
	std::cerr << "In do read header" << std::endl;
	socket_.async_receive(
		boost::asio::buffer(
			recv_buffer_,
			HEADER_LEN
		), boost::bind(
			&Connection::on_read_header_,
			shared_from_this(), 
			_1, _2 						/* errorcode, bytes read */
		)
	);
}

/* On Read, the message is passed to the handler for processing */
void Connection::on_read_header_( error_code error, size_t bytes ) {
	/* If there is no error, read the body */
	if ( ! error ) {

		body_length_ 	= recv_buffer_[ 0 ];
		command_		= recv_buffer_[ 1 ];

		do_read_body_();

	} else {
		if ( error.value() == boost::asio::error::eof ) {
			std::cerr << client_ << " >> EOF." << std::endl;
		}
	}
}

void Connection::do_read_body_( void ) {
	socket_.async_receive( 
		boost::asio::buffer( 
			recv_buffer_,
			body_length_
		), boost::bind( 
			&Connection::on_read_body_,
			shared_from_this(),
			_1, _2
		)
	);
}

void Connection::on_read_body_( error_code error, size_t bytes ) {
	switch( command_ ) {
		case 1: /* TEMPORARY command for login */
		handler_.async_login(
			boost::asio::buffer( recv_buffer_, sizeof( recv_buffer_ )),
			boost::bind( &Connection::on_login_, shared_from_this(), _1 )
		);
		std::cerr << client_ << " >> LOGIN." << std::endl;
		break;
	}	
}

void Connection::do_write_( void ) {
	socket_.async_send(
		boost::asio::buffer(
			send_buffer_,
			strlen( send_buffer_ )
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
	std::memset( send_buffer_, '\0', sizeof( send_buffer_ ));
	do_read_header_();
}