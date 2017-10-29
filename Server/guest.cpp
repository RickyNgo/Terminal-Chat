#include "guest.hpp"

unsigned long Guest::id = 0;

Guest::Guest( void ) : id_( ++id ) { }

Guest::~Guest( void ) { }

const unsigned long Guest::get_id( void ) const {
	return id_;
}

const std::string Guest::get_alias( void ) const {
	return alias_;
}

void Guest::set_alias( const std::string & alias ) {
	alias_ = alias;
}