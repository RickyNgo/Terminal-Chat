#include "session.hpp"

Session::Session( tcp::socket socket, Channel::pointer channel ) 
{
	this->socket = std::move(socket);
	this->read_msg.clear();
	this->start();
} 


Session::~Session( void ) { }

void Session::start( void ) { 
	room.join( shared_from_this());
	
}

void Session::deliver( const std::string & msg ) {
	bool writing = ! write_msg.empty();
	write_msg.push( msg );
	if ( ! writing ) {
		this->do_write();
	}
}

void Session::relay_msg(void)
{
	int cmd = this->read_msg.get_command();

	if (cmd == MSG)
	{
		/*
		Messages to_send = write_msg.pop();
		
		std::string temp;
		
		temp.append(to_send.get_sender());
		
		for (int i = 0; i < 15 - to_send.get_sender().length(); i++)
		{
			temp.append("0");
		}
		
		temp.append(std::to_string(to_send.get_time()));
		
		temp.append(to_send.get_body());
		*/
		//this->room.deliver(write_msg.pop());

		this->do_read();
	}
	else
	{
		this->do_write();
	}
	
}

void Session::do_read_( void ) {
	socket_.async_receive( boost::asio::buffer(read_msg.get_header(), MAX_HEADER_LENGTH), 
	boost::bind( &Session::do_read_header, shared_from_this, _1, _2));
}

void Session::on_read_header( const boost::system::error_code error, size_t bytes ) {
	if ( ! error ) {
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
	this->socket.async_receive(boost::asio::buffer(this->read_msg.get_body(), 
	boost::bind(&Session::on_read_body, shared_from_this(), _1, _2));
}
// MSG
void Session::on_read_body( const boost::system::error_code error, size_t bytes ) {
	if ( ! error ) {
		this->write_msg.push(this->read_msg);

		this->relay_msg();
	}
}

/* Instead of string, should be message class */
void Session::do_write( void ) {
	//std::strcpy( write_buffer_, message_queue_.front().c_str());
	socket.async_send( boost::asio::buffer( this->write_msg.front().get_header(), boost::bind( &Session::on_write, shared_from_this(), _1, _2 ));
}

void Session::on_write( const boost::system::error_code error, size_t bytes ) {
	if ( ! error ) {
		do_read();
	}
}

