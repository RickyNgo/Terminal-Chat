#include "guest.hpp"

unsigned int Guest::id = 0;

Guest::Guest( void ) : id_( ++id ) { }

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

// static bool Guest::compare_alias_( Guest::pointer lhs, Guest::pointer rhs ) {
// 	return ( lhs.get_alias() == rhs.get_alias() ) ? true : false;
// }