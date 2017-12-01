#ifndef __PARTICIPANT_HPP__
#define	__PARTICIPANT_HPP__

#include "boost/shared_ptr.hpp"
#include "../Messages/messages.hpp"

class Participant {
public:
	typedef boost::shared_ptr<Participant> pointer;
	Participant( const char * );
	~Participant( void );

	virtual void deliver( Messages & msg ) = 0;
	const char * alias( void ) const;

private:
	char 	alias_[ 26 ];
};

#endif