#include "session.hpp"

Session::Session( tcp::socket socket, Channel &room)
    : socket_(std::move(socket)), room_(room)
{
	this->read_msg.clear();
} 


Session::~Session( void ) { }

void Session::start( void ) { 
	this->room_.join( shared_from_this() );
	
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

void Session::relay_msg()
{
	int cmd = this->read_msg.get_command();

	if (cmd == MSG)
	{
		// Pass the msg into Channel's msg delivery function

		this->do_read_header();
	}
	else
	{
        // Pass the msg into the Channel's processing function
		this->do_write();
	}
	
}

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

		this->relay_msg();
	}
}

/* Instead of string, should be message class */
void Session::do_write( void ) {
    //std::strcpy( write_buffer_, message_queue_.front().c_str());
    std::string to_send = write_msg.front().get_header();
	socket_.async_send( boost::asio::buffer( to_send), boost::bind( &Session::on_write, shared_from_this(), _1, _2 ));
}

void Session::on_write( const boost::system::error_code error, size_t bytes ) {
	if ( ! error ) {
		do_read_header();
	}
}

