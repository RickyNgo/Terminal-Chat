#ifndef __GUEST_HPP__
#define __GUEST_HPP__

#include <string>
#include <boost/shared_ptr.hpp>

class Guest {
public:
	typedef boost::shared_ptr<Guest> pointer;
	Guest( void );
	virtual ~Guest( void );

	const std::string get_alias( void ) const;
	void set_alias( const std::string & );
	const unsigned long get_id( void ) const;
	virtual void quit() = 0;

private:
	static unsigned long 	id;

	const unsigned long 	id_;
	std::string		 		alias_;

};

#endif