#ifndef __CHANNEL_HPP__
#define __CHANNEL_HPP__
#include <string>
#include <deque>

class Channel :
public enable_shared_from_this<Channel> {
public:
	typedef boost::shared_ptr<Channel> pointer;
	static const unsigned log_max = 100;
	Channel( std::string );
	~Channel( void );

	void join_( User::pointer & );
	void leave_( User::pointer & );
	void deliver_( User::pointer & );

private:
	void process_( void );

	std::string title_;
	std::set<User> users_;
	std::deque<Messages> log_;
};

#endif