#include "guest.hpp"

unsigned int Guest::id = 0;

Guest::Guest( tcp::endpoint address ) :
client_( address ),
id_( ++id ),
alias_set( false )
{ }

Guest::~Guest( void ) { }

const unsigned int Guest::get_id( void ) const {
	return id_;
}

const char * Guest::get_alias( void ) const {
	return alias_;
}

void Guest::set_alias( const char * alias ) {
	if ( ! alias_set ) {
		std::strcpy( alias_, alias );
		alias_set = true;
	}
}
const tcp::endpoint Guest::get_address( void ) const {
	return client_;
}

bool Guest::has_alias( void ) const {
	return alias_set;
}