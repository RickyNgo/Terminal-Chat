#include "session.hpp"
#include "channel.hpp"

Session::Session( Guest::pointer guest, tcp::socket socket, const short port, Channel::pointer channel )
    : Participant( guest->get_alias()), socket_( std::move( socket )), room_( channel ), port_( port ), guest_( guest ), address_( guest->get_address().address(), port_ )
{
	std::cerr << "Session: In Constructor: PORT: " << port_ << std::endl;
	this->read_msg.clear();
} 


Session::~Session( void ) {
	socket_.shutdown( tcp::socket::shutdown_both );
	socket_.close();
}

void Session::start() {
	do_connect_();
}

void Session::deliver( Messages & msg ) {
	bool idle;
	{
		boost::recursive_mutex::scoped_lock lock( write_msg_m_ );
		idle = write_msg.empty();
		write_msg.push( msg );
	}
	if ( idle ) {
		do_write_header_();
	}
}

void Session::do_connect_( void ) {

	socket_.async_connect(
		address_,
		boost::bind(
			&Session::on_connect_,
			shared_from_this(),
			_1
		)
	);
}

void Session::on_connect_( error_code ec ) {
	std::cerr << "Connected to " << address_ << std::endl;
	if ( ! ec ) {
		room_->join( shared_from_this() );
		time_t current_time;
		Messages test( "CHANNEL", "YOU HAVE JOINED", time(&current_time), MSG );
		room_->deliver( test );
		do_read_header_();
	} else {
		std::cerr << "Session::on_connect_(): " << ec.message() << std::endl;
	}
}

void Session::do_read_header_() {
	memset(read_buffer_, '\0', sizeof(char)*512);
	
	socket_.async_receive( boost::asio::buffer( read_buffer_, MAX_HEADER_LENGTH ), 
	boost::bind( &Session::on_read_header_, shared_from_this(), _1, _2));
}

void Session::on_read_header_( const boost::system::error_code error, size_t bytes ) {
	if ( ! error ) {
        std::string temp(read_buffer_);

		if (temp.length() == 0)
		{
			std::cout << temp.length() << std::endl;
			//do_read_header();
			return;
		}
        read_msg.get_header() = std::move(temp);
		read_msg.parse_header();
		do_read_body_();
	} else if ( error.value() == boost::asio::error::eof ) {
		std::cerr << address_ << " >> EOF." << std::endl;
		room_->leave( shared_from_this() );
	}
}

// Will have multiple handlers depending on the command that was sent

void Session::do_read_body_(void)
{
	memset( read_buffer_, '\0', sizeof( char ) * 512 );
	socket_.async_receive( boost::asio::buffer(read_buffer_, MAX_MSG_LENGTH), 
	boost::bind(&Session::on_read_body_, shared_from_this(), _1, _2));
}

void Session::on_read_body_( const boost::system::error_code error, size_t bytes ) {
	
	if ( ! error ) {
        std::string temp( read_buffer_ );
        read_msg.get_body() = std::move( temp );
        switch( read_msg.get_command() ) {
    		case LEAVE:
    			room_->leave( shared_from_this() );
    			break;
    		default:
    			room_->deliver( read_msg );
    			do_read_header_();
    			break;
        }
	} else if ( error.value() == boost::asio::error::eof ) {
		room_->leave( shared_from_this() );
	}
}

/* Instead of string, should be message class */
void Session::do_write_header_( void ) {
    std::string to_send = write_msg.front().get_header();
	socket_.async_send( boost::asio::buffer( to_send, to_send.length()), boost::bind( &Session::on_write_header_, shared_from_this(), _1, _2 ));
}

void Session::on_write_header_( const boost::system::error_code error, size_t bytes ) {
	if ( ! error ) {
		std::string to_send = write_msg.front().get_header();
		do_write_body_();
	}
}

void Session::do_write_body_()
{
	std::string to_send = write_msg.front().get_body();
	socket_.async_send( boost::asio::buffer( to_send, to_send.length()), boost::bind( &Session::on_write_body_, shared_from_this(), _1, _2 ));
}

// void Session::on_write_body_( const boost::system::error_code error, size_t bytes ) {
// 	bool work;
// 	{
// 		boost::recursive_mutex::scoped_lock lock( write_msg_m_ );
// 		write_msg.pop();
// 		work = ! write_msg.empty();
// 	}
// 	if ( ! error && work ) {
// 		std::cerr << "Session::on_write_body_(): work to do" << std::endl;
// 		do_write_header_();
// 	std::cout << "BODY TO SEND " << to_send << std::endl;

// 	boost::asio::async_write(socket_, boost::asio::buffer( to_send, to_send.length()), boost::bind( &Session::on_write_body_, shared_from_this(), _1, _2 ));
// 	}
// }

void Session::on_write_body_( const boost::system::error_code error, size_t bytes ) {
	std::string to_send = write_msg.front().get_body();
	write_msg.pop();

	if ( ! error ) 
	{
		if ( ! write_msg.empty() )
		{
			do_write_header_();
		}	
	}
}



