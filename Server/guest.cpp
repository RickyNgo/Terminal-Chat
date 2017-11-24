#include "guest.hpp"

unsigned int Guest::id = 0;

Guest::Guest( tcp::endpoint address ) :
client_( address ),
id_( ++id )
{ }

Guest::~Guest( void ) { }

const unsigned int Guest::get_id( void ) const {
	return id_;
}

const char * Guest::get_alias( void ) const {
	return alias_;
}

void Guest::set_alias( const char * alias ) {
	std::memcpy( alias_, alias, strlen( alias ));
}
const tcp::endpoint Guest::get_address( void ) const {
	return client_;
}