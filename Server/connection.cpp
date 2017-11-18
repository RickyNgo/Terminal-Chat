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
	std::cerr << client_ << "::STAGE => " << ec << " [" << ec.message() << "]" << std::endl;
	Messages data( "Server", ec.message(), time( NULL ), 0 );
	do_write_header_( data );
}

void Connection::on_login_( error_code ec ) {
	std::cerr << client_ << "::LOGIN => " << ec << " [" << ec.message() << "]" << std::endl;
	Messages data( "Server", ec.message(), time( NULL ), LOGIN );
	do_write_header_( data ); 
}

void Connection::on_leave_( error_code ec ) {
	std::cerr << client_ << "::LEAVE => " << ec << " [" << ec.message() << "]" << std::endl;
	std::cerr << shared_from_this().use_count() - 1 << std::endl;
	/* Leave Scope ... */
}

/* ------------------------------- */
/*       Read-Write Handlers 	   */
/* ------------------------------- */

void Connection::do_read_header_( void ) {
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
	/* If there is no error, read the body */
	if ( ! error ) {

		msg_.get_header() = std::move( read_buffer_ );
		msg_.parse_header();

		do_read_body_();

	} else {
		if ( error.value() == boost::asio::error::eof ) {
			std::cerr << client_ << " >> EOF." << std::endl;
			handler_.async_leave(
				shared_from_this(),
				boost::bind(
					&Connection::on_leave_,
					shared_from_this(),
					_1
				)
			);

		} else {
			std::cerr << error << std::endl;
		}
	}
}

void Connection::do_read_body_( void ) {
	socket_.async_receive( 
		boost::asio::buffer( 
			read_buffer_,
			msg_.get_length()
		), 
		boost::bind( 
			&Connection::on_read_body_,
			shared_from_this(),
			_1, _2
		)
	);
}

void Connection::on_read_body_( error_code error, size_t bytes ) {
	msg_.get_body() = std::move( read_buffer_ );
	switch( msg_.get_command() ) {
		
		case LOGIN:
		
		std::cerr << "alias: " << msg_.get_sender() << std::endl;
		handler_.async_login(
			shared_from_this(),
			const_buffer(
				msg_.get_sender().data(),
				msg_.get_length()
			),
			boost::bind(
				&Connection::on_login_,
				shared_from_this(),
				_1 
			)
		);

		std::cerr << client_ << " >> LOGIN." << std::endl;
		break;

		default:
		do_write_header_( msg_ );
		break;

	}
	
}

void Connection::do_write_header_( Messages msg ) {
	socket_.async_send(
		boost::asio::buffer(
			msg.get_header(),
			MAX_HEADER_LENGTH
		), boost::bind( 
			&Connection::on_write_header_,
			shared_from_this(),
			_1, _2, msg
		)
	);
}


void Connection::on_write_header_( boost::system::error_code error, size_t bytes , Messages msg) {
	if ( ! error ) {
		// std::cerr << bytes << " bytes >> " << client_ << "." << std::endl;
		do_write_body_(msg);
	} else {
		std::cerr << "Connection Error: on_write_: " << error << std::endl;
	}
	
	
}

void Connection::do_write_body_( Messages msg ) {
	std::string data = msg.get_header() + msg.get_body();
	socket_.async_send(
		boost::asio::buffer(
			msg.get_body(),
			msg.get_length()
		), boost::bind( 
			&Connection::on_write_body_,
			shared_from_this(),
			_1, _2
		)
	);
}


void Connection::on_write_body_( boost::system::error_code error, size_t bytes ) {
	if ( ! error ) {
		std::cerr << bytes << " bytes >> " << client_ << "." << std::endl;
	} else {
		std::cerr << "Connection Error: on_write_: " << error << std::endl;
	}
	
	do_read_header_();
}