#ifndef __CHANNEL_HPP__
#define __CHANNEL_HPP__
#include <deque>
#include <set>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include "session.hpp"

class Channel :
public boost::enable_shared_from_this<Channel> {
public:
	typedef boost::shared_ptr<Channel> pointer;
	static const unsigned log_max = 100;
	Channel( void );
	~Channel( void );

	void join( Session::pointer );
	void leave( Session::pointer );
	void deliver( boost::shared_ptr<Messages> );

private:
	void process_( void );

	std::set<Session::pointer>		connections_;
	std::deque<Messages> 	 		log_;
};

#endif