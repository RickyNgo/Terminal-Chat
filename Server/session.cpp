#include "session.hpp"
#include "channel.hpp"

Session::Session( Guest::pointer guest, tcp::socket socket, const short port, Channel::pointer channel )
    : socket_( std::move( socket )), room_( channel ), port_( port ), guest_( guest )
{
	this->read_msg.clear();
	// do_connect_();
} 


Session::~Session( void ) { }

void Session::do_connect_( void ) {
	socket_.async_connect(
		tcp::endpoint(
			guest_->get_address().address(),
			port_
		),
		boost::bind(
			&Session::on_connect_,
			shared_from_this(),
			_1
		)
	);
}

void Session::on_connect_( error_code ec ) {
	if ( ! ec ) {
		room_->join( shared_from_this() );
	}
}

/*
void Session::deliver( const std::string & msg ) {
	bool writing = ! write_msg.empty();
	write_msg.push( msg );
	if ( ! writing ) {
		this->do_write();
	}
}
*/


void Session::do_read_header() {
	this->socket_.async_receive( boost::asio::buffer(this->read_buffer, MAX_MSG_LENGTH), 
	boost::bind( &Session::on_read_header, shared_from_this(), _1, _2));
}

void Session::on_read_header( const boost::system::error_code error, size_t bytes ) {
	if ( ! error ) {
        std::string temp(read_buffer.begin(), read_buffer.end());
        this->read_msg.get_header() = std::move(temp);
		this->read_msg.parse_header();
		this->do_read_body();
	}
	else
	{
		//room.leave(shared_from_this());
	}
}

// Will have multiple handlers depending on the command that was sent

void Session::do_read_body(void)
{
	this->socket_.async_receive( boost::asio::buffer(this->read_buffer, MAX_MSG_LENGTH), 
	boost::bind(&Session::on_read_body, shared_from_this(), _1, _2));
}
// MSG
void Session::on_read_body( const boost::system::error_code error, size_t bytes ) {
	if ( ! error ) {

        std::string temp(read_buffer.begin(), read_buffer.end());
        this->read_msg.get_body() = std::move(temp);
		this->write_msg.push(this->read_msg);

		//this->relay_msg();

		room_->deliver( write_msg.front() );
	}
}

/* Instead of string, should be message class */
void Session::do_write_header( void ) {
    //std::strcpy( write_buffer_, message_queue_.front().c_str());
    std::string to_send = write_msg.front().get_header();
	socket_.async_send( boost::asio::buffer( to_send, to_send.length()), boost::bind( &Session::on_write_header, shared_from_this(), _1, _2 ));
}

void Session::on_write_header( const boost::system::error_code error, size_t bytes ) {
	if ( ! error ) {
		do_write_body();
	}
}

void Session::do_write_body()
{
	std::string to_send = write_msg.front().get_body();
	socket_.async_send( boost::asio::buffer( to_send, to_send.length()), boost::bind( &Session::on_write_body, shared_from_this(), _1, _2 ));
}

void Session::on_write_body( const boost::system::error_code error, size_t bytes ) {
	if ( ! error ) {
		write_msg.pop();
	}
}


