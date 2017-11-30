#ifndef __CHANNEL_HPP__
#define __CHANNEL_HPP__

#include <set>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <deque>

#include "messages.hpp"
#include "channel_utility.hpp"

class Session;

class Channel :
public boost::enable_shared_from_this<Channel> {
public:
	// typedef boost::shared_ptr<Channel> pointer;
	typedef boost::shared_ptr<Channel> pointer;
	static const unsigned log_max = 100;
	Channel( const char * );
	~Channel( void );
	const char * name( void ) const;

	void join( boost::shared_ptr<Session> );
	void leave( boost::shared_ptr<Session> );
	void deliver( Messages );
	void process_( Messages );
	
private:
	std::set<boost::shared_ptr<Session>>		connections_;
	boost::recursive_mutex						connections_m_;
	std::deque<Messages> 	 					log_;
	boost::recursive_mutex						log_m_;
	char 										name_[ 25 ];
};

#endif