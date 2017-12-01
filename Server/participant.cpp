#include "participant.hpp"

Participant::Participant( const char * alias ) {
	std::strcpy( alias_, alias );
}

Participant::~Participant( void ) { }

const char * Participant::alias( void ) const {
	return alias_;
}