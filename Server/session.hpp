#ifndef __SESSION_HPP__
#define __SESSION_HPP__

#include "connection.hpp"

using boost::asio::ip::tcp;

class Session :
public boost::enable_shared_from_this<Session> {
public:
	typedef boost::shared_ptr<Session> pointer;
	void deliver( const std::string & );
	void start( void );
	Session( tcp::socket );
	~Session( void );

};

#endif