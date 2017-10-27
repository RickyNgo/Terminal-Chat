#ifndef __CHANNEL_HPP__
#define __CHANNEL_HPP__
#include <deque>
#include <set>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>

class Channel :
public boost::enable_shared_from_this<Channel> {
public:
	typedef boost::shared_ptr<Channel> pointer;
	static const unsigned log_max = 100;
	Channel( void );
	~Channel( void );

	// void join( Connection::pointer );
	// void leave( Connection::pointer );
	// void deliver( boost::shared_ptr<Messages> );

// private:
	// void process_( void );

	// std::set<Connection::pointer>	connections_;
	// std::deque<Messages> 	 		log_;
};

#endif