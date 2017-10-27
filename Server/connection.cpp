#include "connection.hpp"

/* Set id to zero */
unsigned long Connection::id = 0;

/* Constructor */
Connection::Connection( tcp::socket socket, tcp::endpoint client, ConnectionHandler & handler ) :
socket_( std::move( socket )),
client_( client ),
handler_( handler ),
id_( ++id )
{ }

/* Destructor */
Connection::~Connection( void ) { }

void Connection::start( void ) {

	/* Start reading from connection */
	std::cerr << "tcp connection started with " << client_ << std::endl;
	handler_.connect( shared_from_this() );
	do_read_();
}

void Connection::do_read_( void ) {
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

void Connection::stop( void ) {
	socket_.close();
	std::cerr << "connection with [" << client_ << "] is closed." << std::endl;
}

const unsigned long Connection::get_id( void ) {
	return id_;
}