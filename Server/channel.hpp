#ifndef __CHANNEL_HPP__
#define __CHANNEL_HPP__

#include <set>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <deque>

#include "messages.hpp"
#include "channel_utility.hpp"

class Session;

class Channel :
public boost::enable_shared_from_this<Channel> {
public:
	// typedef boost::shared_ptr<Channel> pointer;
	typedef boost::shared_ptr<Session> pointer;
	static const unsigned log_max = 100;
	Channel( void );
	~Channel( void );

	void join( boost::shared_ptr<Session> );
	void leave( boost::shared_ptr<Session> );
	void deliver( Messages );
	void process_( Messages );
	
private:
	std::set<boost::shared_ptr<Session>>		connections_;
	std::deque<Messages> 	 		log_;
};

#endif