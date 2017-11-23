#ifndef __GUEST_HPP__
#define __GUEST_HPP__

#include <cstring>
#include <boost/shared_ptr.hpp>

class Guest {
public:
	typedef boost::shared_ptr<Guest> 		pointer;
	Guest( void );
	virtual ~Guest( void );

	const char * 			get_alias( void ) const;
	void					set_alias( const char * );
	const unsigned int 		get_id( void ) const;

private:
	static unsigned int 	id;

	const unsigned int 		id_;
	char 	 				alias_[ 25 ];	
};

#endif