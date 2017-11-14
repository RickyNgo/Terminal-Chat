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

/* ------------------------------- */
/*        Public Interface         */
/* ------------------------------- */

void Connection::start( void ) {
	handler_.async_stage(
		shared_from_this(),
		boost::bind(
			&Connection::on_stage_,
			shared_from_this(),
			_1
		)
	); 
}

/* ------------------------------- */
/*   Command Completion Handlers   */
/* ------------------------------- */

void Connection::on_stage_( error_code ec ) {
	if ( ! ec ) {

		std::string body = "Hello. Your connection # is " + std::to_string( get_id()) + ".";
		Messages data( "Server", body, time( NULL ), 1 );
		do_write_( data );
	}
}

void Connection::on_login_( error_code ec ) {
	if ( ! ec ) {

		std::string body = "Your login was successful.";
		Messages data( "Server", body, time( NULL ), 2 );
		do_write_( data );
	}
}

/* ------------------------------- */
/*       Read-Write Handlers 	   */
/* ------------------------------- */

void Connection::do_read_header_( void ) {
	std::cerr << "do_read_header_()" << std::endl;
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
	Messages msg;
	
	msg.get_header() = std::move( read_buffer_ );
	msg.parse_header();

	std::cout << "header body length: " << msg.get_length() << "\nby member: " << msg.get_sender() << std::endl;
    std::cout << "\nheader command: " << msg.get_command() << "\ntime: " << msg.get_time() << std::endl;

	/* If there is no error, read the body */
	if ( ! error ) {
		do_read_body_( msg );

	} else {
		if ( error.value() == boost::asio::error::eof ) {
			std::cerr << client_ << " >> EOF." << std::endl;
		} else {
			std::cerr << error << std::endl;
		}
	}
}

void Connection::do_read_body_( Messages msg ) {
	std::cout << "do_read_body: message_legnth: " << msg.get_length() << std::endl;
	socket_.async_receive( 
		boost::asio::buffer( 
			read_buffer_,
			msg.get_length()
		), boost::bind( 
			&Connection::on_read_body_,
			shared_from_this(),
			_1, _2
		)
	);
}

void Connection::on_read_body_( error_code error, size_t bytes ) {
	msg_.get_body() = std::move( read_buffer_ );
	// switch( msg_.get_command() ) {
		
	// 	case 1: /* TEMPORARY command for login */
		
	// 	handler_.async_login(
	// 		boost::asio::buffer(
	// 			read_buffer_,
	// 			msg_.get_length()
	// 		), boost::bind(
	// 			&Connection::on_login_,
	// 			shared_from_this(),
	// 			_1 
	// 		)
	// 	);

	// 	std::cerr << client_ << " >> LOGIN." << std::endl;
	// 	break;
	// }
	handler_.async_login(
		boost::asio::buffer( read_buffer_, msg_.get_length() ),
		boost::bind( &Connection::on_login_, shared_from_this(), _1 )
	);
}

void Connection::do_write_( Messages msg ) {
	std::cerr << "writing: " << msg.get_body() << std::endl;
	std::string data = msg.get_header() + msg.get_body();
	socket_.async_send(
		boost::asio::buffer(
			data,
			data.length()
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