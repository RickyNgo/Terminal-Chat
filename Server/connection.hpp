#ifndef __CONNECTION_FILES__
#define __CONNECTION_FILES__

#ifndef __CONNECTION__
#define __CONNECTION__

#include <boost/asio.hpp>
using boost::asio::ip::tcp;

#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <deque>
#include <list>
#include <map>
#include "messages.hpp"
#include "channel.hpp"
#include "guest.hpp"

class ConnectionHandler;

class Connection : 
public Guest,
public boost::enable_shared_from_this<Connection> {
public:
	typedef boost::shared_ptr<Connection> pointer;
	Connection( tcp::socket, tcp::endpoint, ConnectionHandler & );
	void start( void );
	void quit( void );
	void response( boost::shared_ptr<Messages> );

private:
	void do_read_( void );
	void on_read_( boost::system::error_code error, size_t bytes );
	void do_write_( void );
	void on_write_( boost::system::error_code error, size_t bytes );
	
	char 					recv_buffer_[ 1024 ];
	char 					send_buffer_[ 1024 ];
	tcp::socket				socket_;
	tcp::endpoint			client_;
	ConnectionHandler 		& handler_;
};

#endif  /* END CONNECTION */

#ifndef __CONNECTION_HANDLER__
#define __CONNECTION_HANDLER__

#define HANDLER_OUT true

class ConnectionHandler {
public:
	ConnectionHandler( void );
	~ConnectionHandler( void );

	void join( Guest::pointer );
	void request( const char *, Guest::pointer );
	void stop_all( void );
	void stop( Guest::pointer );

private:
	std::list<Guest::pointer> 					guests_;
	std::map<std::string, Channel::pointer>		channels_;

};

#endif  /* END CONNECTION_HANDLER */
#endif  /* END CONNECTION_FILES */